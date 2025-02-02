#include <Windows.h>
#include <stdio.h>

#define ID_EDITCHILD 1001 // Identifier for the Edit Control 
#define ID_SAVE_BUTTON 1002 // Identifier for the Save Button

LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit;       // Handle to the Edit Control
    static HWND hSaveButton; // Handle to the Save Button

    switch (uMsg) {
    case WM_CREATE: {
        // Create the Edit Control
        hEdit = CreateWindowExW(
            0,                     // Extended styles
            L"EDIT",               // Predefined Edit Control
            L"",                   // Default text
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            10, 10, 480, 400,      // Position and size of the Edit Control
            hWnd,                  // Parent window handle
            (HMENU)ID_EDITCHILD,   // Control identifier
            GetModuleHandleW(NULL), // Handle to the instance
            NULL                   // Pointer not needed
        );

        // Create the Save Button
        hSaveButton = CreateWindowW(
            L"BUTTON",               // Predefined class
            L"Save",                 // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
            10, 420, 100, 30,        // Position and size of the button
            hWnd,                    // Parent window handle
            (HMENU)ID_SAVE_BUTTON,   // Control identifier
            GetModuleHandleW(NULL),  // Handle to the instance
            NULL                     // Pointer not needed
        );
        break;
    }
        
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case ID_SAVE_BUTTON: {
            // Save button clicked
            int length = GetWindowTextLengthW(hEdit) + 1;     // Get text length
            wchar_t* buffer = (wchar_t*)malloc(length * sizeof(wchar_t)); // Allocate buffer
            if (buffer == NULL) {
                MessageBoxW(hWnd, L"Memory allocation failed!", L"Error", MB_OK | MB_ICONERROR);
                break;
            }
            GetWindowTextW(hEdit, buffer, length);       // Get the text from the edit control

            // Save the text to a UTF-16 file
            FILE* file = _wfopen(L"output.txt", L"w, ccs=UTF-8"); // Write in UTF-8 encoding
            if (file) {
                fputws(buffer, file);                    // Write buffer to file
                fclose(file);
                MessageBoxW(hWnd, L"File saved successfully!", L"Success", MB_OK);
            } else {
                MessageBoxW(hWnd, L"Failed to save file.", L"Error", MB_OK | MB_ICONERROR);
            }

            free(buffer); // Free the allocated buffer
            break;
        }
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam); // Unicode version
    }

    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WinProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"NotepadClass";

    RegisterClassW(&wc);

    HWND hWnd = CreateWindowW(
        L"NotepadClass",
        L"Super Pad",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 500, // Adjusted window size
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}
