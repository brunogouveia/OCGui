#ifdef OCGUI_IMPLEMENTATION__

#include <OCGui/InputText.h>

namespace OCGui
{
    static int InputTextCallback(ImGuiTextEditCallbackData* data)
    {
        std::cout << "Callback called" << std::endl;
        return 0;
    }

    InputText::InputText(std::string&& label, InputTextFlags flags, std::function<void(void)> enterCallback, uint bufferSize):
        Widget(std::move(label)),
        m_flags(flags),
        m_buffer(new char[bufferSize]),
        m_bufferSize(bufferSize),
        m_enterCallback(enterCallback)
    {

    }

    InputText::~InputText()
    {
        if (m_buffer)
            delete m_buffer;
    }

    void InputText::Draw()
    {
        using namespace ImGui;

        ImVec2 size_arg(0,0);
        void* user_data = NULL;

        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;

        IM_ASSERT(!((m_flags & ImGuiInputTextFlags_CallbackHistory) && (m_flags & ImGuiInputTextFlags_Multiline))); // Can't use both together (they both use up/down keys)
        IM_ASSERT(!((m_flags & ImGuiInputTextFlags_CallbackCompletion) && (m_flags & ImGuiInputTextFlags_AllowTabInput))); // Can't use both together (they both use tab key)

        ImGuiState& g = *((ImGuiState*)ImGui::GetInternalState());
        const ImGuiIO& io = g.IO;
        const ImGuiStyle& style = g.Style;

        const ImGuiID id = window->GetID(m_label.c_str());
        const bool is_multiline = (m_flags & ImGuiInputTextFlags_Multiline) != 0;
        const bool is_editable = (m_flags & ImGuiInputTextFlags_ReadOnly) == 0;

        ImVec2 label_size = ImGui::CalcTextSize(m_label.c_str(), NULL, true);
        ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), is_multiline ? ImGui::GetTextLineHeight() * 8.0f : label_size.y); // Arbitrary default of 8 lines high for multi-line
        const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size + style.FramePadding*2.0f);
        const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? (style.ItemInnerSpacing.x + label_size.x) : 0.0f, 0.0f));

        ImGuiWindow* draw_window = window;
        if (is_multiline)
        {
            ImGui::BeginGroup();
            if (!ImGui::BeginChildFrame(id, frame_bb.GetSize()))
            {
                ImGui::EndChildFrame();
                ImGui::EndGroup();
                return;
            }
            draw_window = GetCurrentWindow();
            draw_window->DC.CursorPos += style.FramePadding;
            size.x -= draw_window->ScrollbarSizes.x;
        }
        else
        {
            ItemSize(total_bb, style.FramePadding.y);
            if (!ItemAdd(total_bb, &id))
                return;
        }

        // NB: we are only allowed to access 'edit_state' if we are the active widget.
        ImGuiTextEditState& edit_state = g.InputTextState;

        const bool is_ctrl_down = io.KeyCtrl;
        const bool is_shift_down = io.KeyShift;
        const bool is_alt_down = io.KeyAlt;
        const bool focus_requested = FocusableItemRegister(window, g.ActiveId == id, (m_flags & (ImGuiInputTextFlags_CallbackCompletion|ImGuiInputTextFlags_AllowTabInput)) == 0);    // Using completion callback disable keyboard tabbing
        const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
        const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

        const bool hovered = IsHovered(frame_bb, id);
        if (hovered)
        {
            g.HoveredId = id;
            g.MouseCursor = ImGuiMouseCursor_TextInput;
        }
        const bool user_clicked = hovered && io.MouseClicked[0];
        const bool user_scrolled = is_multiline && g.ActiveId == 0 && edit_state.Id == id && g.ActiveIdPreviousFrame == draw_window->GetID("#SCROLLY");

        bool select_all = (g.ActiveId != id) && (m_flags & ImGuiInputTextFlags_AutoSelectAll) != 0;
        if (focus_requested || user_clicked || user_scrolled)
        {
            if (g.ActiveId != id)
            {
                // Start edition
                // Take a copy of the initial buffer value (both in original UTF-8 format and converted to wchar)
                // From the moment we focused we are ignoring the content of 'buf'
                const int prev_len_w = edit_state.CurLenW;
                edit_state.Text.resize(m_bufferSize);        // wchar count <= utf-8 count
                edit_state.InitialText.resize(m_bufferSize); // utf-8
                ImFormatString(edit_state.InitialText.Data, edit_state.InitialText.Size, "%s", m_buffer);
                const char* buf_end = NULL;
                edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, m_buffer, NULL, &buf_end);
                edit_state.CurLenA = (int)(buf_end - m_buffer); // We can't get the result from ImFormatString() above because it is not UTF-8 aware. Here we'll cut off malformed UTF-8.
                edit_state.InputCursorScreenPos = ImVec2(-1.f, -1.f);
                edit_state.CursorAnimReset();

                // Preserve cursor position and undo/redo stack if we come back to same widget
                // FIXME: We should probably compare the whole buffer to be on the safety side. Comparing buf (utf8) and edit_state.Text (wchar).
                const bool recycle_state = (edit_state.Id == id) && (prev_len_w == edit_state.CurLenW);
                if (recycle_state)
                {
                    // Recycle existing cursor/selection/undo stack but clamp position
                    // Note a single mouse click will override the cursor/position immediately by calling stb_textedit_click handler.
                    edit_state.StbState.cursor = ImMin(edit_state.StbState.cursor, edit_state.CurLenW);
                    edit_state.StbState.select_start = ImMin(edit_state.StbState.select_start, edit_state.CurLenW);
                    edit_state.StbState.select_end = ImMin(edit_state.StbState.select_end, edit_state.CurLenW);
                }
                else
                {
                    edit_state.Id = id;
                    edit_state.ScrollX = 0.f;
                    stb_textedit_initialize_state(&edit_state.StbState, !is_multiline);
                    if (!is_multiline && focus_requested_by_code)
                        select_all = true;
                }
                if (m_flags & ImGuiInputTextFlags_AlwaysInsertMode)
                    edit_state.StbState.insert_mode = true;
                if (!is_multiline && (focus_requested_by_tab || (user_clicked && is_ctrl_down)))
                    select_all = true;
            }
            SetActiveID(id, window);
            FocusWindow(window);
        }
        else if (io.MouseClicked[0])
        {
            // Release focus when we click outside
            if (g.ActiveId == id)
                SetActiveID(0);
        }

        bool value_changed = false;
        bool cancel_edit = false;
        bool enter_pressed = false;

        if (g.ActiveId == id)
        {
            edit_state.BufSizeA = m_bufferSize;

            // Although we are active we don't prevent mouse from hovering other elements unless we are interacting right now with the widget.
            // Down the line we should have a cleaner concept of focused vs active in the library.
            g.ActiveIdIsFocusedOnly = !io.MouseDown[0];

            // Edit in progress
            const float mouse_x = (g.IO.MousePos.x - frame_bb.Min.x - style.FramePadding.x) + edit_state.ScrollX;
            const float mouse_y = (is_multiline ? (g.IO.MousePos.y - draw_window->DC.CursorPos.y - style.FramePadding.y) : (g.FontSize*0.5f));

            if (select_all || (hovered && io.MouseDoubleClicked[0]))
            {
                edit_state.SelectAll();
                edit_state.SelectedAllMouseLock = true;
            }
            else if (io.MouseClicked[0] && !edit_state.SelectedAllMouseLock)
            {
                stb_textedit_click(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
                edit_state.CursorAnimReset();
            }
            else if (io.MouseDown[0] && !edit_state.SelectedAllMouseLock)
            {
                stb_textedit_drag(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
                edit_state.CursorAnimReset();
            }
            if (edit_state.SelectedAllMouseLock && !io.MouseDown[0])
                edit_state.SelectedAllMouseLock = false;

            if (g.IO.InputCharacters[0])
            {
                // Process text input (before we check for Return because using some IME will effectively send a Return?)
                if (!is_ctrl_down && is_editable)
                {
                    for (int n = 0; n < IM_ARRAYSIZE(g.IO.InputCharacters) && g.IO.InputCharacters[n]; n++)
                        if (unsigned int c = (unsigned int)g.IO.InputCharacters[n])
                        {
                            // Insert character if they pass filtering
                            if (!InputTextFilterCharacter(&c, m_flags, InputTextCallback, user_data))
                                continue;
                            edit_state.OnKeyPressed((int)c);
                        }
                }

                // Consume characters
                memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));
            }

            const int k_mask = (is_shift_down ? STB_TEXTEDIT_K_SHIFT : 0);
            const bool is_ctrl_only = is_ctrl_down && !is_alt_down && !is_shift_down;
            if (IsKeyPressedMap(ImGuiKey_LeftArrow))                        { edit_state.OnKeyPressed(is_ctrl_down ? STB_TEXTEDIT_K_WORDLEFT | k_mask : STB_TEXTEDIT_K_LEFT | k_mask); }
            else if (IsKeyPressedMap(ImGuiKey_RightArrow))                  { edit_state.OnKeyPressed(is_ctrl_down ? STB_TEXTEDIT_K_WORDRIGHT | k_mask  : STB_TEXTEDIT_K_RIGHT | k_mask); }
            else if (is_multiline && IsKeyPressedMap(ImGuiKey_UpArrow))     { if (is_ctrl_down) SetWindowScrollY(draw_window, draw_window->Scroll.y - g.FontSize); else edit_state.OnKeyPressed(STB_TEXTEDIT_K_UP | k_mask); }
            else if (is_multiline && IsKeyPressedMap(ImGuiKey_DownArrow))   { if (is_ctrl_down) SetWindowScrollY(draw_window, draw_window->Scroll.y + g.FontSize); else edit_state.OnKeyPressed(STB_TEXTEDIT_K_DOWN| k_mask); }
            else if (IsKeyPressedMap(ImGuiKey_Home))                        { edit_state.OnKeyPressed(is_ctrl_down ? STB_TEXTEDIT_K_TEXTSTART | k_mask : STB_TEXTEDIT_K_LINESTART | k_mask); }
            else if (IsKeyPressedMap(ImGuiKey_End))                         { edit_state.OnKeyPressed(is_ctrl_down ? STB_TEXTEDIT_K_TEXTEND | k_mask : STB_TEXTEDIT_K_LINEEND | k_mask); }
            else if (IsKeyPressedMap(ImGuiKey_Delete) && is_editable)      { edit_state.OnKeyPressed(STB_TEXTEDIT_K_DELETE | k_mask); }
            else if (IsKeyPressedMap(ImGuiKey_Backspace) && is_editable)   { edit_state.OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask); }
            else if (IsKeyPressedMap(ImGuiKey_Enter))
            {
                bool ctrl_enter_for_new_line = (m_flags & ImGuiInputTextFlags_CtrlEnterForNewLine) != 0;
                if (!is_multiline || (ctrl_enter_for_new_line && !is_ctrl_down) || (!ctrl_enter_for_new_line && is_ctrl_down))
                {
                    if (!(m_flags & InputTextFlags_KeepActiveOnEnter))
                    {
                        SetActiveID(0);
                    }
                    enter_pressed = true;
                }
                else if (is_editable) // New line
                {
                    unsigned int c = '\n';
                    if (InputTextFilterCharacter(&c, m_flags, InputTextCallback, user_data))
                        edit_state.OnKeyPressed((int)c);
                }
            }
            else if ((m_flags & ImGuiInputTextFlags_AllowTabInput) && IsKeyPressedMap(ImGuiKey_Tab) && !is_ctrl_down && !is_shift_down && !is_alt_down && is_editable)
            {
                unsigned int c = '\t';
                if (InputTextFilterCharacter(&c, m_flags, InputTextCallback, user_data))
                    edit_state.OnKeyPressed((int)c);
            }
            else if (IsKeyPressedMap(ImGuiKey_Escape))                              { SetActiveID(0); cancel_edit = true; }
            else if (is_ctrl_only && IsKeyPressedMap(ImGuiKey_Z) && is_editable)    { edit_state.OnKeyPressed(STB_TEXTEDIT_K_UNDO); edit_state.ClearSelection(); }
            else if (is_ctrl_only && IsKeyPressedMap(ImGuiKey_Y) && is_editable)    { edit_state.OnKeyPressed(STB_TEXTEDIT_K_REDO); edit_state.ClearSelection(); }
            else if (is_ctrl_only && IsKeyPressedMap(ImGuiKey_A))                   { edit_state.SelectAll(); edit_state.CursorFollow = true; }
            else if (is_ctrl_only && ((IsKeyPressedMap(ImGuiKey_X) && is_editable) || IsKeyPressedMap(ImGuiKey_C)) && (!is_multiline || edit_state.HasSelection()))
            {
                // Cut, Copy
                const bool cut = IsKeyPressedMap(ImGuiKey_X);
                if (cut && !edit_state.HasSelection())
                    edit_state.SelectAll();

                if (g.IO.SetClipboardTextFn)
                {
                    const int ib = edit_state.HasSelection() ? ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end) : 0;
                    const int ie = edit_state.HasSelection() ? ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end) : edit_state.CurLenW;
                    edit_state.TempTextBuffer.resize((ie-ib) * 4 + 1);
                    ImTextStrToUtf8(edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data+ib, edit_state.Text.Data+ie);
                    g.IO.SetClipboardTextFn(edit_state.TempTextBuffer.Data);
                }

                if (cut)
                {
                    edit_state.CursorFollow = true;
                    stb_textedit_cut(&edit_state, &edit_state.StbState);
                }
            }
            else if (is_ctrl_only && IsKeyPressedMap(ImGuiKey_V) && is_editable)
            {
                // Paste
                if (g.IO.GetClipboardTextFn)
                {
                    if (const char* clipboard = g.IO.GetClipboardTextFn())
                    {
                        // Remove new-line from pasted buffer
                        const int clipboard_len = (int)strlen(clipboard);
                        ImWchar* clipboard_filtered = (ImWchar*)ImGui::MemAlloc((clipboard_len+1) * sizeof(ImWchar));
                        int clipboard_filtered_len = 0;
                        for (const char* s = clipboard; *s; )
                        {
                            unsigned int c;
                            s += ImTextCharFromUtf8(&c, s, NULL);
                            if (c == 0)
                                break;
                            if (c >= 0x10000 || !InputTextFilterCharacter(&c, m_flags, InputTextCallback, user_data))
                                continue;
                            clipboard_filtered[clipboard_filtered_len++] = (ImWchar)c;
                        }
                        clipboard_filtered[clipboard_filtered_len] = 0;
                        if (clipboard_filtered_len > 0) // If everything was filtered, ignore the pasting operation
                        {
                            stb_textedit_paste(&edit_state, &edit_state.StbState, clipboard_filtered, clipboard_filtered_len);
                            edit_state.CursorFollow = true;
                        }
                        ImGui::MemFree(clipboard_filtered);
                    }
                }
            }

            if (cancel_edit)
            {
                // Restore initial value
                if (is_editable)
                {
                    ImFormatString(m_buffer, m_bufferSize, "%s", edit_state.InitialText.Data);
                    value_changed = true;
                }
            }
            else
            {
                // Apply new value immediately - copy modified buffer back
                // Note that as soon as we can focus into the input box, the in-widget value gets priority over any underlying modification of the input buffer
                // FIXME: We actually always render 'buf' in RenderTextScrolledClipped
                // FIXME-OPT: CPU waste to do this every time the widget is active, should mark dirty state from the stb_textedit callbacks
                if (is_editable)
                {
                    edit_state.TempTextBuffer.resize(edit_state.Text.Size * 4);
                    ImTextStrToUtf8(edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data, NULL);
                }

                // User callback
                if ((m_flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackAlways)) != 0)
                {
                    IM_ASSERT(InputTextCallback != NULL);

                    // The reason we specify the usage semantic (Completion/History) is that Completion needs to disable keyboard TABBING at the moment.
                    ImGuiInputTextFlags event_flag = 0;
                    ImGuiKey event_key = ImGuiKey_COUNT;
                    if ((m_flags & ImGuiInputTextFlags_CallbackCompletion) != 0 && IsKeyPressedMap(ImGuiKey_Tab))
                    {
                        event_flag = ImGuiInputTextFlags_CallbackCompletion;
                        event_key = ImGuiKey_Tab;
                    }
                    else if ((m_flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_UpArrow))
                    {
                        event_flag = ImGuiInputTextFlags_CallbackHistory;
                        event_key = ImGuiKey_UpArrow;
                    }
                    else if ((m_flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_DownArrow))
                    {
                        event_flag = ImGuiInputTextFlags_CallbackHistory;
                        event_key = ImGuiKey_DownArrow;
                    }

                    if (event_key != ImGuiKey_COUNT || (m_flags & ImGuiInputTextFlags_CallbackAlways) != 0)
                    {
                        ImGuiTextEditCallbackData callback_data;
                        callback_data.EventFlag = event_flag;
                        callback_data.Flags = m_flags;
                        callback_data.UserData = user_data;
                        callback_data.ReadOnly = !is_editable;

                        callback_data.EventKey = event_key;
                        callback_data.Buf = edit_state.TempTextBuffer.Data;
                        callback_data.BufSize = edit_state.BufSizeA;
                        callback_data.BufDirty = false;

                        // We have to convert from position from wchar to UTF-8 positions
                        ImWchar* text = edit_state.Text.Data;
                        const int utf8_cursor_pos = callback_data.CursorPos = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.cursor);
                        const int utf8_selection_start = callback_data.SelectionStart = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_start);
                        const int utf8_selection_end = callback_data.SelectionEnd = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_end);

                        // Call user code
                        InputTextCallback(&callback_data);

                        // Read back what user may have modified
                        IM_ASSERT(callback_data.Buf == edit_state.TempTextBuffer.Data);  // Invalid to modify those fields
                        IM_ASSERT(callback_data.BufSize == edit_state.BufSizeA);
                        IM_ASSERT(callback_data.Flags == m_flags);
                        if (callback_data.CursorPos != utf8_cursor_pos)            edit_state.StbState.cursor = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.CursorPos);
                        if (callback_data.SelectionStart != utf8_selection_start)  edit_state.StbState.select_start = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionStart);
                        if (callback_data.SelectionEnd != utf8_selection_end)      edit_state.StbState.select_end = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionEnd);
                        if (callback_data.BufDirty)
                        {
                            edit_state.CurLenW = ImTextStrFromUtf8(text, edit_state.Text.Size, edit_state.TempTextBuffer.Data, NULL);
                            edit_state.CurLenA = (int)strlen(edit_state.TempTextBuffer.Data);
                            edit_state.CursorAnimReset();
                        }
                    }
                }

                if (is_editable && strcmp(edit_state.TempTextBuffer.Data, m_buffer) != 0)
                {
                    ImFormatString(m_buffer, m_bufferSize, "%s", edit_state.TempTextBuffer.Data);
                    value_changed = true;
                }
            }
        }

        if (!is_multiline)
            RenderFrame(frame_bb.Min, frame_bb.Max, window->Color(ImGuiCol_FrameBg), true, style.FrameRounding);

        ImVec2 render_pos = is_multiline ? draw_window->DC.CursorPos : frame_bb.Min + style.FramePadding;

        ImVec4 clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x + style.FramePadding.x*2.0f, frame_bb.Min.y + size.y + style.FramePadding.y*2.0f);
        ImVec2 text_size(0.f, 0.f);
        if (g.ActiveId == id || (edit_state.Id == id && is_multiline && g.ActiveId == draw_window->GetID("#SCROLLY")))
        {
            edit_state.CursorAnim += g.IO.DeltaTime;

            // We need to:
            // - Display the text (this can be more easily clipped)
            // - Handle scrolling, highlight selection, display cursor (those all requires some form of 1d->2d cursor position calculation)
            // - Measure text height (for scrollbar)
            // We are attempting to do most of that in **one main pass** to minimize the computation cost (non-negligible for large amount of text) + 2nd pass for selection rendering (we could merge them by an extra refactoring effort)
            const ImWchar* text_begin = edit_state.Text.Data;
            const ImWchar* text_end = text_begin + edit_state.CurLenW;
            ImVec2 cursor_offset, select_start_offset;

            {
                // Count lines + find lines numbers of cursor and select_start
                int matches_remaining = 0;
                int matches_line_no[2] = { -1, -999 };
                const ImWchar* matches_ptr[2] = { NULL, NULL };
                matches_ptr[0] = text_begin + edit_state.StbState.cursor; matches_remaining++;
                if (edit_state.StbState.select_start != edit_state.StbState.select_end)
                {
                    matches_ptr[1] = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
                    matches_line_no[1] = -1;
                    matches_remaining++;
                }
                matches_remaining += is_multiline ? 1 : 0;     // So that we never exit the loop until all lines are counted.

                int line_count = 0;
                for (const ImWchar* s = text_begin; s < text_end+1; s++)
                    if ((*s) == '\n' || s == text_end)
                    {
                        line_count++;
                        if (matches_line_no[0] == -1 && s >= matches_ptr[0]) { matches_line_no[0] = line_count; if (--matches_remaining <= 0) break; }
                        if (matches_line_no[1] == -1 && s >= matches_ptr[1]) { matches_line_no[1] = line_count; if (--matches_remaining <= 0) break; }
                    }

                // Calculate 2d position
                IM_ASSERT(matches_line_no[0] != -1);
                cursor_offset.x = InputTextCalcTextSizeW(ImStrbolW(matches_ptr[0], text_begin), matches_ptr[0]).x;
                cursor_offset.y = matches_line_no[0] * g.FontSize;
                if (matches_line_no[1] >= 0)
                {
                    select_start_offset.x = InputTextCalcTextSizeW(ImStrbolW(matches_ptr[1], text_begin), matches_ptr[1]).x;
                    select_start_offset.y = matches_line_no[1] * g.FontSize;
                }

                // Calculate text height
                if (is_multiline)
                    text_size = ImVec2(size.x, line_count * g.FontSize);
            }

            // Scroll
            if (edit_state.CursorFollow)
            {
                // Horizontal scroll in chunks of quarter width
                if (!(m_flags & ImGuiInputTextFlags_NoHorizontalScroll))
                {
                    const float scroll_increment_x = size.x * 0.25f;
                    if (cursor_offset.x < edit_state.ScrollX)
                        edit_state.ScrollX = ImMax(0.0f, cursor_offset.x - scroll_increment_x);
                    else if (cursor_offset.x - size.x >= edit_state.ScrollX)
                        edit_state.ScrollX = cursor_offset.x - size.x + scroll_increment_x;
                }
                else
                {
                    edit_state.ScrollX = 0.0f;
                }

                // Vertical scroll
                if (is_multiline)
                {
                    float scroll_y = draw_window->Scroll.y;
                    if (cursor_offset.y - g.FontSize < scroll_y)
                        scroll_y = ImMax(0.0f, cursor_offset.y - g.FontSize);
                    else if (cursor_offset.y - size.y >= scroll_y)
                        scroll_y = cursor_offset.y - size.y;
                    draw_window->DC.CursorPos.y += (draw_window->Scroll.y - scroll_y);   // To avoid a frame of lag
                    draw_window->Scroll.y = scroll_y;
                    render_pos.y = draw_window->DC.CursorPos.y;
                }
            }
            edit_state.CursorFollow = false;
            ImVec2 render_scroll = ImVec2(edit_state.ScrollX, 0.0f);

            // Draw selection
            if (edit_state.StbState.select_start != edit_state.StbState.select_end)
            {
                const ImWchar* text_selected_begin = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
                const ImWchar* text_selected_end = text_begin + ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end);

                float bg_offy_up = is_multiline ? 0.0f : -1.0f;    // FIXME: those offsets should be part of the style? they don't play so well with multi-line selection.
                float bg_offy_dn = is_multiline ? 0.0f : 2.0f;
                ImVec2 rect_pos = render_pos + select_start_offset - render_scroll;
                for (const ImWchar* p = text_selected_begin; p < text_selected_end; )
                {
                    if (rect_pos.y > clip_rect.w + g.FontSize)
                        break;
                    if (rect_pos.y < clip_rect.y)
                    {
                        while (p < text_selected_end)
                            if (*p++ == '\n')
                                break;
                    }
                    else
                    {
                        ImVec2 rect_size = InputTextCalcTextSizeW(p, text_selected_end, &p, NULL, true);
                        if (rect_size.x <= 0.0f) rect_size.x = (float)(int)(g.Font->GetCharAdvance((unsigned short)' ') * 0.50f); // So we can see selected empty lines
                        ImRect rect(rect_pos + ImVec2(0.0f, bg_offy_up - g.FontSize), rect_pos +ImVec2(rect_size.x, bg_offy_dn));
                        rect.Clip(clip_rect);
                        // if (rect.Overlaps(clip_rect))
                        //     draw_window->DrawList->AddRectFilled(rect.Min, rect.Max, bg_color);
                    }
                    rect_pos.x = render_pos.x - render_scroll.x;
                    rect_pos.y += g.FontSize;
                }
            }

            // draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos - render_scroll, draw_window->Color(ImGuiCol_Text), buf, buf+edit_state.CurLenA, 0.0f, is_multiline ? NULL : &clip_rect);
            float xs,ys;
            xs = frame_bb.Max.x - frame_bb.Min.x;
            ys = frame_bb.Max.y - frame_bb.Min.y;
            if (edit_state.HasSelection())
            {
                bndTextField(GetNVGcontext(), frame_bb.Min.x, frame_bb.Min.y, xs, ys, 0, BND_ACTIVE, -1, m_buffer, ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end), ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end));
            }
            else if (m_flags & InputTextFlags_ReadOnly) {
                bndTextField(GetNVGcontext(), frame_bb.Min.x, frame_bb.Min.y, xs, ys, BND_CORNER_NONE, BND_DEFAULT, -1, m_buffer, 0, -1);
            }
            else
            {
                bndTextField(GetNVGcontext(), frame_bb.Min.x, frame_bb.Min.y, xs, ys, 0, BND_ACTIVE, -1, m_buffer, edit_state.StbState.cursor, edit_state.StbState.cursor);
            }

            // Draw blinking cursor
            ImVec2 cursor_screen_pos = render_pos + cursor_offset - render_scroll;
//            bool cursor_is_visible = (g.InputTextState.CursorAnim <= 0.0f) || fmodf(g.InputTextState.CursorAnim, 1.20f) <= 0.80f;
            // if (cursor_is_visible)
            //     draw_window->DrawList->AddLine(cursor_screen_pos + ImVec2(0.0f,-g.FontSize+0.5f), cursor_screen_pos + ImVec2(0.0f,-1.5f), window->Color(ImGuiCol_Text));

            // Notify OS of text input position for advanced IME
            if (is_editable && io.ImeSetInputScreenPosFn && ImLengthSqr(edit_state.InputCursorScreenPos - cursor_screen_pos) > 0.0001f)
                io.ImeSetInputScreenPosFn((int)cursor_screen_pos.x - 1, (int)(cursor_screen_pos.y - g.FontSize));   // -1 x offset so that Windows IME can cover our cursor. Bit of an extra nicety.

            edit_state.InputCursorScreenPos = cursor_screen_pos;
        }
        else
        {
            // Render text only
            const char* buf_end = NULL;
            if (is_multiline)
                text_size = ImVec2(size.x, InputTextCalcTextLenAndLineCount(m_buffer, &buf_end) * g.FontSize); // We don't need width
            // draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, draw_window->Color(ImGuiCol_Text), buf, buf_end, 0.0f, is_multiline ? NULL : &clip_rect);
            float xs,ys;
            xs = frame_bb.Max.x - frame_bb.Min.x;
            ys = frame_bb.Max.y - frame_bb.Min.y;
            bndTextField(GetNVGcontext(), frame_bb.Min.x, frame_bb.Min.y, xs, ys, 0, (m_flags & InputTextFlags_ReadOnly) ? BND_DEFAULT : BND_ACTIVE, -1, m_buffer, 0, -1);
        }

        if (is_multiline)
        {
            ImGui::Dummy(text_size + ImVec2(0.0f, g.FontSize)); // Always add room to scroll an extra line
            ImGui::EndChildFrame();
            ImGui::EndGroup();
        }

        if (enter_pressed)
        {
            m_enterCallback();
        }
    }

    void InputText::SetText(std::string&& text)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGuiState& g = *((ImGuiState*)ImGui::GetInternalState());
        
        const ImGuiID id = window->GetID(m_label.c_str());
        
        if (g.ActiveId == id)
        {
            // NB: we are only allowed to access 'edit_state' if we are the active widget.
            ImGuiTextEditState& editState = g.InputTextState;
            editState.SelectAll();
            editState.OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE);
            
            for (char c: text)
            {
                editState.OnKeyPressed((int)c);
            }
            
        }
        
        // Clear buffer
        strcpy(m_buffer, text.c_str());
    }

    void InputText::ClearText()
    {
        SetText("");
    }

} /* OCGui */

#endif
