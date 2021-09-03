#pragma once


/* Declaration */
bool BeginPiePopup(const char *pName, int iMouseButton = 0);
void EndPiePopup();

bool PieMenuItem(const char *pName, bool bEnabled = true);
bool BeginPieMenu(const char *pName, bool bEnabled = true);
void EndPieMenu();

int PiePopupSelectMenu(const ImVec2& center, const char* popup_id, const char** items, int items_count, int* p_selected);
