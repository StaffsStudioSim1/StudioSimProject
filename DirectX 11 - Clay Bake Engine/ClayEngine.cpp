#include "ClayEngine.h"

bool ClayEngine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	Keyboard.EnableAutoRepeatChars(); // ENABLES char auto repeat for output example of default class use 
	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
	{
		return false;
	}
	if (!gamefx.Initialize(this->render_window.GetHWND(), width, height))
	{
		return false;
	}

	return true;
	// initialise graphics here 
}

bool ClayEngine::ProcessMessages()
{
	return this->render_window.proccessMessages();
}

void ClayEngine::Update()
{

	while (!Keyboard.CharBufferIsEmpty())
	{	// way to output current char pressed	shows just letter pressed
		unsigned char ch = Keyboard.ReadChar();	 // read key
		std::string outmsg = "char: "; outmsg += ch; outmsg += "\n"; // compile complete string 
		OutputDebugStringA(outmsg.c_str());	// output string 
	}
	while (!Keyboard.KeyBufferIsEmpty())
	{	// way to output current Key pressed shows key event of down / up
		KeyboardEvent kbe = Keyboard.ReadKey(); // read keyboard event
		unsigned char keycode = kbe.GetKeyCode();	// get key pressed
		std::string outmsg = "key: "; outmsg += keycode; outmsg += "\n"; // compile string for output
		OutputDebugStringA(outmsg.c_str()); // output string 
	}

	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent(); // read mouse event the mouse moment here is just basic pos on screen 
		std::string outmsg = "X:"; outmsg += std::to_string(me.GetPosX()); outmsg += ", Y: "; outmsg += std::to_string(me.GetPosY()); outmsg += "\n";	// read mouse event the mouse moment here is
		OutputDebugStringA(outmsg.c_str()); // output string																					//  just basic pos on screen then compile mouse string for output
		if(me.GetType() == MouseEvent::EventType::WheelUp)
		{
			OutputDebugStringA("mouse wheel up.\n");
		}
		if (me.GetType() == MouseEvent::EventType::WheelDown) // scroll wheel examples
		{
			OutputDebugStringA("mouse wheel down.\n");
		}

		if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
		{
			std::string outmsg = "X: "; outmsg += std::to_string(me.GetPosX()); outmsg += ", Y: "; outmsg += std::to_string(me.GetPosY()); outmsg += "\n"; // this string settup uses raw mouse movment
			OutputDebugStringA(outmsg.c_str());
		}

	}

}

void ClayEngine::RenderFrame()
{
	gamefx.RenderFrame();
}