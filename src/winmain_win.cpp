/* Windows-only: WinMain-Einstieg, ruft main() auf. Getrennt von main.cpp, damit dort kein windows.h nötig ist. */
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

extern int main(int argc, char* argv[]);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  (void)hInstance;
  (void)hPrevInstance;
  (void)lpCmdLine;
  (void)nCmdShow;
  return main(__argc, __argv);
}
#endif
