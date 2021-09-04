// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context
// creation, etc.) If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

// #include "IconsFontAwesome4.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <ImGui/ImGui.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#  define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <ImGui/ImGuiInternal.h>
#include <stdio.h>

#include <ImGui/Widget/ImGuiTextEditor.h>
#include <fstream>
#include <streambuf>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#  include <GLES2/gl2.h>
// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#  include <GL/gl3w.h> // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#  include <GL/glew.h> // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#  include <glad/glad.h> // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#  include <glad/gl.h> // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#  define GLFW_INCLUDE_NONE      // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#  include <glbinding/Binding.h> // Initialize with glbinding::Binding::initialize()
#  include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#  define GLFW_INCLUDE_NONE // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#  include <glbinding/gl/gl.h>
#  include <glbinding/glbinding.h> // Initialize with glbinding::initialize()

using namespace gl;
#else
#  include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and
// compatibility with old VS compilers. To link with VS2010-era libraries, VS2015+ requires linking with
// legacy_stdio_definitions.lib, which we do using this pragma. Your own project should not be affected, as you are
// likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#  pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char *description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void beginTextColor()
{
  ImGuiStyle *style     = &ImGui::GetStyle();
  ImVec4 *    colors    = style->Colors;
  colors[ImGuiCol_Text] = ImVec4(1.f, 0.f, 0.f, 1.00f);
}

void endTextColor()
{
  ImGuiStyle *style     = &ImGui::GetStyle();
  ImVec4 *    colors    = style->Colors;
  colors[ImGuiCol_Text] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
}

void myApplyDefaultStyle()
{
  ImGuiStyle *style  = &ImGui::GetStyle();
  ImVec4 *    colors = style->Colors;

  colors[ImGuiCol_Text]                  = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_TextDisabled]          = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_WindowBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
  colors[ImGuiCol_Border]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.54f);
  colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.21f, 0.21f, 0.21f, 0.40f);
  colors[ImGuiCol_FrameBgActive]         = ImVec4(0.34f, 0.34f, 0.34f, 0.67f);
  colors[ImGuiCol_TitleBg]               = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
  colors[ImGuiCol_TitleBgActive]         = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
  colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
  colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
  colors[ImGuiCol_CheckMark]             = ImVec4(0.32f, 0.62f, 0.75f, 1.00f);
  colors[ImGuiCol_SliderGrab]            = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
  colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.60f, 0.70f, 0.75f, 1.00f);
  colors[ImGuiCol_Button]                = ImVec4(0.34f, 0.34f, 0.34f, 0.40f);
  colors[ImGuiCol_ButtonHovered]         = ImVec4(0.38f, 0.38f, 0.38f, 0.78f);
  colors[ImGuiCol_ButtonActive]          = ImVec4(0.29f, 0.42f, 0.63f, 1.00f);
  colors[ImGuiCol_Header]                = ImVec4(0.26f, 0.26f, 0.26f, 0.40f);
  colors[ImGuiCol_HeaderHovered]         = ImVec4(0.38f, 0.38f, 0.38f, 0.78f);
  colors[ImGuiCol_HeaderActive]          = ImVec4(0.29f, 0.42f, 0.63f, 1.00f);
  colors[ImGuiCol_Separator]             = ImVec4(0.35f, 0.32f, 0.31f, 0.50f);
  colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
  colors[ImGuiCol_SeparatorActive]       = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
  colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
  colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
  colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
  colors[ImGuiCol_Tab]                   = ImVec4(0.14f, 0.14f, 0.14f, 0.86f);
  colors[ImGuiCol_TabHovered]            = ImVec4(0.34f, 0.50f, 0.76f, 1.00f);
  colors[ImGuiCol_TabActive]             = ImVec4(0.34f, 0.50f, 0.76f, 1.00f);
  colors[ImGuiCol_TabUnfocused]          = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
  colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
  colors[ImGuiCol_DockingPreview]        = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
  colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
  colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
  colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
  colors[ImGuiCol_TableBorderLight]      = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
  colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
  colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
  colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
  colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

  style->ChildRounding     = 4.0f;
  style->FrameBorderSize   = 1.0f;
  style->FrameRounding     = 1.0f;
  style->GrabMinSize       = 12.0f;
  style->PopupRounding     = 1.0f;
  style->ScrollbarRounding = 12.0f;
  style->ScrollbarSize     = 13.0f;
  style->TabBorderSize     = 1.0f;
  style->TabRounding       = 2.0f;
  style->WindowRounding    = 4.0f;
  style->ItemSpacing       = ImVec2(8.f, 5.f);
  style->WindowRounding    = 1.f;
}

void SetDockSpace()
{

  static bool               opt_fullscreen  = true;
  static bool               opt_padding     = false;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
  // because it would be confusing to have two docking targets within each others.
  //   ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
  if (opt_fullscreen)
  {
    // const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImGuiViewport *viewport = ImGui::GetWindowViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
                    | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  }
  else
  {
    dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
  }

  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
  // and handle the pass-thru hole, so we ask Begin() to not render a background.
  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;

  // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
  // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
  // all active windows docked into it will lose their parent and become undocked.
  // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
  // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
  if (!opt_padding) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace", nullptr, window_flags);
  if (!opt_padding) ImGui::PopStyleVar();

  if (opt_fullscreen) ImGui::PopStyleVar(2);

  ImGuiID DockspaceID = ImGui::GetID("Dockspace");

  if (!ImGui::DockBuilderGetNode(DockspaceID))
  {
    ImGui::DockBuilderRemoveNode(DockspaceID); // Clear out existing layout
    ImGui::DockBuilderAddNode(DockspaceID);    // Add empty node
    ImGui::DockBuilderSetNodeSize(DockspaceID, ImGui::GetIO().DisplaySize * ImGui::GetIO().DisplayFramebufferScale);

    ImGuiID dock_main_id = DockspaceID;
    ImGuiID DockLeft  = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.5f, nullptr, &dock_main_id);
    ImGuiID DockRight = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, nullptr, &dock_main_id);

    ImGui::DockBuilderDockWindow("AST View", DockRight);
;
    ImGui::DockBuilderDockWindow("Text Editor", DockLeft);


    ImGui::DockBuilderFinish(DockspaceID);
  }

  // Submit the DockSpace
  ImGuiIO &io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
  {
    // ImGuiID dockspace_id = ImGui::GetID("MyDockSpace1");
    ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f), dockspace_flags);
  }
  ImGui::End();
}

#include <clang-c/CXString.h>
#include <clang-c/Index.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

TextEditor editor;

CXChildVisitResult visitor(CXCursor cursor, CXCursor /* parent */, CXClientData clientData)
{
  CXSourceLocation location = clang_getCursorLocation(cursor);
  if (clang_Location_isFromMainFile(location) == 0) return CXChildVisit_Continue;

  CXCursorKind cursorKind = clang_getCursorKind(cursor);

  unsigned int curLevel  = *(reinterpret_cast<unsigned int *>(clientData));
  unsigned int nextLevel = curLevel + 1;

  const char *a = clang_getCString(clang_getCursorKindSpelling(cursorKind));
  const char *b = clang_getCString(clang_getCursorSpelling(cursor));
  std::string c = clang_getCString(clang_getTypeSpelling(clang_getCursorType(cursor)));

  std::string show_string
      = "|" + std::string(curLevel, '-') + std::string(a) + "  " + std::string(c) + " <" + std::string(b) + ">";


  CXFile   file;
  unsigned line;
  unsigned column;
  unsigned offset;

  clang_getFileLocation(location, &file, &line, &column, &offset);

  if (ImGui::Button(show_string.c_str())) editor.SetCursorPosition({int(line-1), int(column-1)});
  clang_visitChildren(cursor, visitor, &nextLevel);

  return CXChildVisit_Continue;
}

int main(int, char **)
{

  std::string temp_file_name = "temp.cpp";

  // WriteFile(temp_file_name,init_code);
  CXIndex           idx = clang_createIndex(1, 1);
  CXTranslationUnit tu;
  bool              clang_unit_init = false;

  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char *glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

  // Create window with graphics context
  GLFWwindow *window = glfwCreateWindow(1280, 720, "Clang AST Viewer", NULL, NULL);
  if (window == NULL) return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
  bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
  bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
  bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
  bool err = gladLoadGL(glfwGetProcAddress)
             == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
  bool err = false;
  glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
  bool err = false;
  glbinding::initialize([](const char *name) { return (glbinding::ProcAddress) glfwGetProcAddress(name); });
#else
  bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of
                    // initialization.
#endif
  if (err)
  {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    return 1;
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void) io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

  myApplyDefaultStyle();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  io.Fonts->AddFontFromFileTTF(
      "Font/Consolas-with-Yahei Nerd Font.ttf", 26.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
  ImFontConfig config;
  config.MergeMode                   = true;
  config.GlyphMinAdvanceX            = 13.0f; // Use if you want to make the icon monospaced
  static const ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
  io.Fonts->AddFontFromFileTTF("Font/fontawesome-webfont.ttf", 20.0f, &config, icon_ranges);

  // Our state
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // TEXT EDITOR SAMPLE

  auto lang = TextEditor::LanguageDefinition::CPlusPlus();
  editor.SetLanguageDefinition(lang);

  static const char *fileToEdit = "temp.cpp";

  // Main loop
  while (!glfwWindowShouldClose(window))
  {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your
    // inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those
    // two flags.
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    SetDockSpace();

    {
      auto cpos = editor.GetCursorPosition();
      //   ImGui::Begin("Text Editor Demo", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
      ImGui::Begin("Text Editor", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);
      ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
      if (ImGui::BeginMenuBar())
      {
        if (ImGui::BeginMenu("File"))
        {
          auto f = [&]
          {
            std::string textToSave = editor.GetText();
            // WriteFile(std::string("temp.cpp"),textToSave);
            std::ofstream myfile;
            myfile.open(temp_file_name);
            myfile << textToSave;
            myfile.close();
            tu              = clang_createTranslationUnitFromSourceFile(idx, "temp.cpp", 0, 0, 0, 0);
            clang_unit_init = true;
            // editor.SetCursorPosition({1,1});
            return true;
          };
          if (ImGui::MenuItem("Save", "Ctrl+S")) { f(); }
          if (ImGui::MenuItem("Quit", "Alt-F4")) break;
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
          bool ro = editor.IsReadOnly();
          if (ImGui::MenuItem("Read-only mode", nullptr, &ro)) editor.SetReadOnly(ro);
          ImGui::Separator();

          if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo())) editor.Undo();
          if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo())) editor.Redo();

          ImGui::Separator();

          if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection())) editor.Copy();
          if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection())) editor.Cut();
          if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection())) editor.Delete();
          if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr)) editor.Paste();

          ImGui::Separator();

          if (ImGui::MenuItem("Select all", nullptr, nullptr))
            editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

          ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
          if (ImGui::MenuItem("Dark palette")) editor.SetPalette(TextEditor::GetDarkPalette());
          if (ImGui::MenuItem("Light palette")) editor.SetPalette(TextEditor::GetLightPalette());
          if (ImGui::MenuItem("Retro blue palette")) editor.SetPalette(TextEditor::GetRetroBluePalette());
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }

      ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s",
                  cpos.mLine + 1,
                  cpos.mColumn + 1,
                  editor.GetTotalLines(),
                  editor.IsOverwrite() ? "Ovr" : "Ins",
                  editor.CanUndo() ? "*" : " ",
                  editor.GetLanguageDefinition().mName.c_str(),
                  fileToEdit);

      editor.Render("TextEditor");
      ImGui::End();
    }

    {

      ImGui::Begin("AST View", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

      unsigned int treeLevel = 0;
      if (clang_unit_init)
      {
        clang_visitChildren(clang_getTranslationUnitCursor(tu), visitor, &treeLevel);
        clang_unit_init = true;
      }

      ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(
        clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste
    // this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
