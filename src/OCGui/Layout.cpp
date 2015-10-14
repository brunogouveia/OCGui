#ifdef OCGUI_IMPLEMENTATION__

#include <OCGui/Layout.h>

namespace OCGui
{

    void BeginArea(const Size& size)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        
        window->DC.GroupStack.resize(window->DC.GroupStack.Size + 1);
        ImGuiGroupData& group_data = window->DC.GroupStack.back();
        group_data.GroupSize = size;
        group_data.BackupCursorPos = window->DC.CursorPos;
        group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
        group_data.BackupColumnsStartX = window->DC.ColumnsStartX;
        group_data.BackupCurrentLineHeight = window->DC.CurrentLineHeight;
        group_data.BackupCurrentLineTextBaseOffset = window->DC.CurrentLineTextBaseOffset;
        group_data.BackupLogLinePosY = window->DC.LogLinePosY;
        group_data.AdvanceCursor = true;
        
        window->DC.ColumnsStartX = window->DC.CursorPos.x - window->Pos.x;
        window->DC.CursorMaxPos = window->DC.CursorPos;
        window->DC.CurrentLineHeight = 0.0f;
        window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;
    }
    
    void EndArea()
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGuiStyle& style = ImGui::GetStyle();
        
        IM_ASSERT(!window->DC.GroupStack.empty());
        
        ImGuiGroupData& group_data = window->DC.GroupStack.back();
        
        ImRect group_bb(group_data.BackupCursorPos, window->DC.CursorMaxPos);
        group_bb.Max.y -= style.ItemSpacing.y;      // Cancel out last vertical spacing because we are adding one ourselves.
        group_bb.Max = ImMax(group_bb.Min, group_bb.Max);
        
        window->DC.CursorPos = group_data.BackupCursorPos;
        window->DC.CursorMaxPos = ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
        window->DC.CurrentLineHeight = group_data.BackupCurrentLineHeight;
        window->DC.CurrentLineTextBaseOffset = group_data.BackupCurrentLineTextBaseOffset;
        window->DC.ColumnsStartX = group_data.BackupColumnsStartX;
        window->DC.LogLinePosY = window->DC.CursorPos.y - 9999.0f;
        
        if (group_data.AdvanceCursor)
        {
            window->DC.CurrentLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset, group_data.BackupCurrentLineTextBaseOffset);      // FIXME: Incorrect, we should grab the base offset from the *first line* of the group but it is hard to obtain now.
            ImGui::ItemSize(group_bb.GetSize(), group_data.BackupCurrentLineTextBaseOffset);
            ImGui::ItemAdd(group_bb, NULL);
        }
        
        window->DC.GroupStack.pop_back();
        
        //window->DrawList->AddRect(group_bb.Min, group_bb.Max, 0xFFFF00FF);   // Debug
    }
    
    Size GetCurrentAreaSize()
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        IM_ASSERT(!window->DC.GroupStack.empty());
        ImGuiGroupData& group_data = window->DC.GroupStack.back();
        return group_data.GroupSize;
    }

}

#endif
