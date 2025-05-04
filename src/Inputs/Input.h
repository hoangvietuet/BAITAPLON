#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"
enum Axis {HORIZONTAL, VERICAL};
class Input
{
    public:
 static Input* GetInstance(){
      return s_Instance = (s_Instance != nullptr)? s_Instance : new Input();

   }

        void Listen();
        bool GetKeyDown(SDL_Scancode key);
        int GetAxisKey(Axis axis);

    private:
        Input();
        void KeyUp();
        void KeyDown();
         //bool m_KeyStates[SDL_NUM_SCANCODES]; // Lưu trạng thái phím
        const Uint8* m_KeyStates;
        static Input* s_Instance;
};

#endif // INPUT_H
