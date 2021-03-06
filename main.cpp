
#include "RayEngine.h"

#include "SDL\SDL.h"

// for debugging
#include <iostream>


// note:
// - tree seems to be valid?

// fix the crash
// //engine->importObj("C:\\Users\\Rob\\3d\\torus.obj"); do crash... TODO< fix it>

// check tree build algorithm
//    (cos its just untested)
//    (check the tree that it puts out for validity)
// build tree-walk algorithm
//    (i use a simple non-tree algorithm, rebuild it to a "raw" Element-walk and the to a real tree-walk)
// < check if the matrix code is correct >
// TODO< do overwork the verticles/polygons/BSC implementation (use our container types, build in exceptions where something can fail) >

// other problems that may get delayed?
//  NONE

// done:
// - 24.6.10 openGL interop
// - 25.6.10 normal interpolarisation

using namespace std;

#ifdef _WIN32
#undef main
#endif 

int main(int argc, char *argv[]) {
	cout << offsetof(BVH_Content,polysc) << endl;
	cout << offsetof(rays2,ppx) << endl;
	cout << sizeof(struct rays2_) << endl;
	cout << sizeof(struct poly) << endl;
	
	unsigned long xres = 1024; // is the x resolution
	unsigned long yres = 768; // is the y resolution
	
	// this is for sdl
	SDL_Surface*         screen;
	const SDL_VideoInfo* info;
	SDL_Event            event;
	
	// initialisize the sdl and the video output
	if( SDL_Init(SDL_INIT_VIDEO) == -1 ) {
		printf("Can't initialisize SDL!\r\n");
		return 1;
	}
		
	// switch to the best videomode
	info = SDL_GetVideoInfo();
	if( !info ) {
		printf("Can't get the Video mode!\r\n");
		return 1;
	}
	
	Uint32 flags = SDL_OPENGL|SDL_DOUBLEBUF; // video flags
		
	if( info->hw_available ) {
		flags |= SDL_HWSURFACE;
	}
	else {
		flags |= SDL_SWSURFACE;
	}

	if(info->blit_hw) {
		flags |= SDL_HWACCEL;
	}
	
	//if(fullscreen)
	//	flags |= SDL_FULLSCREEN;
	
	
	// Set Opengl Values
	long color;

	color = 10; // for 32 bits
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,	 color);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,	 color);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,	 color);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,		16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,	 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,	 8);

	// Set Title
	SDL_WM_SetCaption(0, 0);
	
	// open the Screen
	screen = SDL_SetVideoMode(xres, yres, 32, flags);
	if( !screen ) {
		printf("Can't open the screen!\r\n");
		return 1;
	}

	// set up the Ray Engine
	RayEngine* engine = new RayEngine();
	
	/*
	// try to import the file
	engine->importObj("C:\\Users\\Rob\\superbox.2obj.obj");


	// test the build sphere algorithm
	engine->buildSpheres();

	// do transfer the engine triangle/verticle/bsc data into an Mesh
	// TODO< just do this by default if something is imported and return the Mesh of it >
	RayEngine::Mesh* mymesh = engine->createMesh();
	
	// create a object from the mesh
	RayEngine::Object* myobject = engine->createObject(mymesh);
	*/






	
	// try to import the file
	//engine->importObj("C:\\Users\\Rob\\3d\\plant.obj");
	//engine->importObj("C:\\Users\\Rob\\3d\\torus.obj"); do crash... TODO< fix it>
	//engine->importObj("C:\\Users\\Rob\\3d\\monkey.obj"); do crash
	
	//engine->importObj("C:\\Users\\Rob\\3d\\cone.obj");
	engine->importObj("C:\\Users\\Rob\\3d\\sphere.obj");


	// test the build sphere algorithm
	engine->buildSpheres();

	// do transfer the engine triangle/verticle/bsc data into an Mesh
	// TODO< just do this by default if something is imported and return the Mesh of it >
	RayEngine::Mesh* mymesh2 = engine->createMesh();
	
	// create a object from the mesh
	RayEngine::Object* myobject2 = engine->createObject(mymesh2);

	
	myobject2->rotation.x = 0.6f;




	RayEngine::Camera* camera = new RayEngine::Camera();
	
	camera->position.x = -0.62f;
	camera->position.y = 0.16f;
	camera->position.z = 0.15f;
	
	camera->look.x = 1.0f;
	camera->look.y = 0.0f;
	camera->look.z = 0.0f;
	
	camera->up.x = 0.0f;
	camera->up.y = 1.0f;
	camera->up.z = 0.0f;

	engine->camera = camera;

	engine->setResolution(xres, yres);
	
	unsigned long fps = 0; // a counter for the fps
	unsigned long fps_last_ticks = SDL_GetTicks();
	
	SDL_WM_GrabInput(SDL_GRAB_ON);
	
	int oldX, oldY;

	SDL_GetMouseState(&oldX, &oldY);
	
	unsigned long lastFrame; // is the absolute time of the last frame

	// mainloop
	for(;;) {
		// fps thing
		unsigned long actualTicks = SDL_GetTicks();
		
		
		lastFrame = actualTicks;

		if( actualTicks - fps_last_ticks >= 1000 ) {
			// ouput the average frametime:
			cout << "AVG-time: " << static_cast<float>(actualTicks - fps_last_ticks)/static_cast<float>(fps) << endl;
			
			fps_last_ticks = actualTicks;

			// display/update the fps
			cout << fps << endl;

			//cout << Global.cameraXPos << endl;

			fps = 0;
		}

		fps += 1;
		
		engine->render(screen);

		// switch the buffers
		////////////////////////////////////////////////SDL_Flip(screen);

		//SDL_WarpMouse(500, 500);

		// get the close-event
		while (SDL_PollEvent(&event)) {
		}
		if( event.type == SDL_QUIT ) {
			break;
		}
		else if( event.type == SDL_MOUSEMOTION ) {
			
			//int x,y;

			//SDL_GetMouseState(&x, &y);
		
			// calculate the relative mouse motion
			float xd = static_cast<float>(oldX-event.motion.x) * 0.005f;
			float yd = static_cast<float>(oldY-event.motion.y) * 0.005f;
			
			oldX = event.motion.x;
			oldY = event.motion.y;

			// calculate the new view direction
			Vector3 camX, camY;

			camera->getCameraX(camX);
			camera->getCameraY(camY);
			
			// we don't move the camera cos we want to benchmark
			///camera->look.x = camera->look.x + camX.x * xd + camY.x * yd;
			///camera->look.y = camera->look.y + camX.y * xd + camY.y * yd;
			///camera->look.z = camera->look.z + camX.z * xd + camY.z * yd;

			
		}

		Uint8 *keys;
		keys = SDL_GetKeyState(NULL);
		if( keys[SDLK_UP] ) {
			
			camera->position.x = camera->position.x + camera->look.x * 0.01f;
			camera->position.y = camera->position.y + camera->look.y * 0.01f;
			camera->position.z = camera->position.z + camera->look.z * 0.01f;
			/*
			// TODO
			camera->position.x += 0.01f;
			//Global.cameraBeginX = 1.0 + Global.cameraXPos;
			*/
		}
		else if( keys[SDLK_DOWN] ) {
			camera->position.x = camera->position.x - camera->look.x * 0.01f;
			camera->position.y = camera->position.y - camera->look.y * 0.01f;
			camera->position.z = camera->position.z - camera->look.z * 0.01f;


			/*
			// TODO
			camera->position.x  -= 0.01f;
			//Global.cameraBeginX = 1.0 + Global.cameraXPos;*/
		}
		else if( keys[SDLK_LEFT] ) {
			Vector3 camX;
			
			camera->getCameraX(camX);

			camera->position.x = camera->position.x + camX.x * 0.01f;
			camera->position.y = camera->position.y + camX.y * 0.01f;
			camera->position.z = camera->position.z + camX.z * 0.01f;
		}
		else if( keys[SDLK_RIGHT] ) {
			Vector3 camX;
			
			camera->getCameraX(camX);

			camera->position.x = camera->position.x - camX.x * 0.01f;
			camera->position.y = camera->position.y - camX.y * 0.01f;
			camera->position.z = camera->position.z - camX.z * 0.01f;
		}
		else if( keys[SDLK_ESCAPE] ) {
			break;
		}
		
		
		///SDL_WarpMouse(500, 500);
		
		//myobject->rotation.x += 0.05f;
		//myobject->rotation.y += 0.1f;
		/*
		myobject->rotation.x += 0.03f;
		myobject->rotation.y += 0.05f;
		myobject->rotation.z += 0.07f;

		myobject->position.x = 30.0f;
		*/
		////myobject->rotation.x += 0.03f;
		////myobject->rotation.y += 0.05f;
	}

	// clean all up
	SDL_FreeSurface(screen);
	SDL_Quit();
	
	return 0;
}
