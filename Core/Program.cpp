#include <stdio.h>
#include <time.h>
#include <Engine.h>
#include "Loader.h"
#include <ModelGL.h>
#include <Datatypes.h>

#include "Timer.h"

// Constants and global vars.
Engine* engine;
bool mousein = false;
ModelGL* m2;
long lastTime=0;
int mode = 3;


int counter = 0;
double elapsed = 0.0;
Timer timer;

/* Returns the predicted frames per second. */
void showFPS() {

  // Update FPS every second.
  counter ++;  
  elapsed = timer.getElapsedTime();
  if (elapsed > 1.0) {
    double fps = counter / elapsed;
    counter = 0;
    timer.start ();
    printf ("FPS: %4.1f\n", fps);
  }
}


/* Evaluates the SDL events. */
bool EvaluateEvents () {
  SDL_Event event;
  while (SDL_PollEvent (&event)) { // Get all messages.
    switch (event.type) {
      
    case SDL_MOUSEBUTTONDOWN:
      mousein = true;
      SDL_ShowCursor (SDL_DISABLE);
      if (event.button.button == SDL_BUTTON_LEFT)
        printf ("[%d | %d] Linke Maustaste gedrückt.\n", event.button.x, event.button.y);
        break;

    case SDL_MOUSEMOTION:
      /*
      printf ("Maus um (%d | %d) Pixel auf (%d | %d) bewegt. Tasten: %s %s %s\n",
              event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y,
              (event.motion.state & SDL_BUTTON_LMASK ? "[Links] "  : ""),
              (event.motion.state & SDL_BUTTON_RMASK ? "[Rechts] " : ""),
              (event.motion.state & SDL_BUTTON_MMASK ? "[Mitte] "  : ""));   */   
      break;

    //case SDL_KEYUP:   // Keyboardnachrichten abfangen (kein break -> SDL_KEYUP
    case SDL_KEYDOWN: //  und SDL_KEYDOWN werden gleich behandelt)
      if(event.key.keysym.sym == SDLK_c     // Taste [C] gedrückt
      && event.key.keysym.mod & KMOD_CTRL ) { // [STRG] gedrückt
        printf ("Ctrl-C pressed.\n");
        mousein = false;
        SDL_ShowCursor (SDL_ENABLE);
      }

      switch (event.key.keysym.sym) {
        case SDLK_0: m2->ChangeMode (0);   break;
        case SDLK_1: m2->ChangeMode (1);   break;
        case SDLK_2: m2->ChangeMode (2);   break;
        case SDLK_3: m2->ChangeMode (3);   break;
        case SDLK_4: m2->ChangeMode (4);   break;
        case SDLK_7: engine->SetResolution (640, 480, false);   break;
        case SDLK_8: engine->SetResolution (800, 600, false);   break;
        case SDLK_9: engine->SetResolution ( -1,  -1,  true);   break;
      } 
      
      if (event.key.keysym.sym != SDLK_ESCAPE) break; // Alles außer [ESC] ignorieren

      // Quit command.
      case SDL_QUIT:
        return false;

      // Irrelevant event type.
      default:
        break;

    }  
  }
  return true;
}


void dev0 () {  
  //Loader loader = Loader();
  //loader.loadMDX ("..\\Debug\\in\\Cat.mdx");  
  ModelGL* m2 = new ModelGL (85, NULL);
  m2->LoadFile ("..\\Debug\\out\\Bear.mod");
  m2->Echo (false);
  m2->Echo (true);
  getchar ();
}


void dev1 () {  
  Loader loader = Loader ();
  //Texture* texture = loader.LoadTexture ("..\\Debug\\in\\bsb2.tga");
  m2 = new ModelGL (85, NULL);
  m2->LoadFile ("..\\Debug\\out\\Bear.mod");
  m2->Echo (false);
  m2->Echo (true);
  engine->AddModel (m2);
  engine->camera->SetPosition (0, 0, 20, 0, 0);

  glEnable (GL_LIGHTING);   // Enable lighting.
  glEnable (GL_LIGHT0);
  float lightpos [] = {600.0f, 400.0f, 300.0f, 0.0f}; // Position. 4th is directional (0) or positional. 
  float diffuseLight [] = {1.0f, 1.0f, 1.0f, 1.0f};   // Color for diffuse light (RGBA code).
  float ambientLight [] = {0.2f, 0.2f, 0.2f, 1.0f};   // Color for ambient (environmental) light.
  float specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};   // Specular (reflected) light.
  glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuseLight);    // Set diffuse light.
  glLightfv (GL_LIGHT0, GL_AMBIENT, ambientLight);    // Set ambient light.
  glLightfv (GL_LIGHT0, GL_SPECULAR, specularLight);  // Set specular light.  
}



void dev2 () {
  engine->camera->SetPosition (100, 65, 120, 38, -8);
  Loader loader = Loader ();
  //Texture* texture = loader.LoadTexture ("..\\Debug\\in\\bsb2.tga");
  //m2 = new ModelGL (85, texture);
  //m2->LoadFile ("..\\Debug\\out\\Bear.mod");

  ModelGL* m3 = new ModelGL (84, NULL);
  m3->LoadFile ("..\\Debug\\out\\draken.mod");
  
  //m2->Echo (false);
  m3->Echo (false);

  //engine->AddModel (m2);
  engine->AddModel (m3);

  glEnable (GL_LIGHTING);   // Enable lighting.
  glEnable (GL_LIGHT0);
  float lightpos [] = {600.0f, 400.0f, 300.0f, 0.0f}; // Position. 4th is directional (0) or positional. 
  float diffuseLight [] = {1.0f, 1.0f, 1.0f, 1.0f};   // Color for diffuse light (RGBA code).
  float ambientLight [] = {0.2f, 0.2f, 0.2f, 1.0f};   // Color for ambient (environmental) light.
  float specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};   // Specular (reflected) light.
  glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuseLight);    // Set diffuse light.
  glLightfv (GL_LIGHT0, GL_AMBIENT, ambientLight);    // Set ambient light.
  glLightfv (GL_LIGHT0, GL_SPECULAR, specularLight);  // Set specular light.  
}


void dev3 () {
  Loader loader = Loader ();
  Texture* texture = loader.LoadTexture ("..\\Debug\\in\\texture.jpg");
  m2 = new ModelGL (85, texture);
  m2->LoadFile ("..\\Debug\\out\\hf.mod");

  engine->AddModel (m2);
  engine->camera->SetPosition (450, 300, 350, -40, 28);
}



int main (int argc, char** argv) { 

  // Console version shall be used.
  if (mode == 0) dev0 ();
  
  // Gentlemen, start the engines!
  else {
    engine = new Engine ("OpenGL Engine", 800, 600, false);
    switch (mode) {
      case 1: dev1 (); break;
      case 2: dev2 (); break;
      case 3: dev3 (); break;
    }

    // Main execution loop.
    while (true) { 
      if (EvaluateEvents ()) { 
        engine->camera->MouseIn = mousein;  // TODO Work-around.
        float lp1 [] = {-70, -30, 100, 1}; 
        glLightfv (GL_LIGHT0, GL_POSITION, lp1);
        engine->Render ();
        //system("cls");
        showFPS ();
      }    
      else break;
    }
    engine->~Engine();  
  }

  return 0;
}


/*
  glEnable (GL_LIGHTING);   // Enable lighting.
  glEnable (GL_LIGHT0);
  float lightpos [] = {600.0f, 400.0f, 300.0f, 0.0f};    // Position. 4th is directional (0) or positional. 
  float diffuseLight [] = {1.0f, 1.0f, 1.0f, 1.0f};  // Color for diffuse light (RGBA code).
  float ambientLight [] = {0.2f, 0.2f, 0.2f, 1.0f};  // Color for ambient (environmental) light.
  float specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};  // Specular (reflected) light.
  glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuseLight);   // Set diffuse light.
  glLightfv (GL_LIGHT0, GL_AMBIENT, ambientLight);   // Set ambient light.
  glLightfv (GL_LIGHT0, GL_SPECULAR, specularLight); // Set specular light.

  glEnable  (GL_LIGHT1);
  glLightfv (GL_LIGHT1, GL_DIFFUSE, diffuseLight);   // Set diffuse light.
  glLightfv (GL_LIGHT1, GL_AMBIENT, ambientLight);   // Set ambient light.
  glLightfv (GL_LIGHT1, GL_SPECULAR, specularLight); // Set specular light.

      float lp1 [] = {300.0f, 300.0f, 200.0f, 0.0f};
      float lp2 [] = {700.0f, 280.0f, 500.0f, 0.0f};
      glLightfv (GL_LIGHT0, GL_POSITION, lp1);
      glLightfv (GL_LIGHT0, GL_POSITION, lp2);


ExecGameFunctions:
  //glCallList (objListNr);

  //DrawQuad (tex, 100.0f, 100.0f);
  //TextureSlice texs (tex, 20.0f, 20.0f, 80.0f, 80.0f);
  //DrawQuad (&texs, 200.0f, 100.0f);






float x = 0;
float z = 0.0f;
float entf = -10.0f;
float speed = 0.0f;
bool out = true;
bool up = true;


void BuildPlane () {
  z += 1.0f;
  if (z >= 360.0f) z -= 360.0f;

  glTranslatef (0.0,0.0,-5.0);
  glRotatef (z,1.0,1.0,1.0);
  //glBindTexture (GL_TEXTURE_2D, tex);
  glBegin (GL_QUADS);
    glTexCoord2f (0.0,2.0);
    glVertex3f (-2.0,2.0,0.0);
    glTexCoord2f (0.0,0.0);
    glVertex3f (-2.0,-2.0,0.0);
    glTexCoord2f (2.0,0.0);
    glVertex3f (2.0,-2.0,0.0);
    glTexCoord2f (2.0,2.0);
    glVertex3f (2.0,2.0,0.0);
  glEnd ();
}       

void BuildTriangle () {
  if (up) x += 0.01f;
  else x -= 0.01f;
  if (x >= 1.0f || x <= 0.0f) up = !up;
  glClearColor (x,x,x, 0.0f);
  SDL_Delay(40);

  glBegin (GL_TRIANGLES);   // Wir wollen ein Dreieck zeichnen
 
    glColor3f (1,0,0);      // Ab jetzt werden alle gezeichneten Punkte rot
    glVertex3f (0.5,0.25,0); // Der erste Eckpunkt ist mittig und 100 Pixel
                            // vom oberen Rand entfernt

    glColor3f (0,1,0);      // Ab jetzt werden alle gezeichneten Punkte grün
    glVertex3f (0.75,0.75,0); // Der zweite Eckpunkt ist 50 Pixel vom rechten
                            // und 100 Pixel vom unteren Rand entfernt
 
    glColor3f (0,0,1);      // Ab jetzt werden alle gezeichneten Punkte blau
    glVertex3f (0.25,0.75,0);  // Der dritte Eckpunkt ist 50 Pixel vom linken
                            // und 100 Pixel vom unteren Rand entfernt
  glEnd ();                 // Wir sind fertig mit dem Zeichnen
}

void BuildQuader () {
  
  float difamb [] = {1.0f, 0.5f, 0.3f, 1.0f};
  glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, difamb);

  glBegin( GL_QUADS ); // Wir bauen den Würfel aus Quadraten (Quads) auf
    glNormal3f (0.0f, 0.0f, -1.0f);
    //glColor3f(1, 0,   0  );   // Ab jetzt werden alle gezeichneten Punkte rot 
      glVertex3f( 1,  1, -1);
      glVertex3f( 1, -1, -1);
      glVertex3f(-1, -1, -1);
      glVertex3f(-1,  1, -1);

    glNormal3f (0.0f, 0.0f, 1.0f);
    //glColor3f(0, 1,   0  );   // Ab jetzt werden alle gezeichneten Punkte grün
      glVertex3f( 1,  1,  1);
      glVertex3f(-1,  1,  1);
      glVertex3f(-1, -1,  1);
      glVertex3f( 1, -1,  1);

    glNormal3f (1.0f, 0.0f, 0.0f);
    //glColor3f(0, 0,   1  );
      glVertex3f( 1,  1, -1);
      glVertex3f( 1,  1,  1);
      glVertex3f( 1, -1,  1);
      glVertex3f( 1, -1, -1);

    glNormal3f (-1.0f, 0.0f, 0.0f);
    //glColor3f(0, 0.5, 1  );
      glVertex3f(-1, -1, -1);
      glVertex3f(-1, -1,  1);
      glVertex3f(-1,  1,  1);
      glVertex3f(-1,  1, -1);

    glNormal3f (0.0f, -1.0f, 0.0f);
    //glColor3f(1, 1,   0  );
      glVertex3f( 1, -1, -1);
      glVertex3f( 1, -1,  1);
      glVertex3f(-1, -1,  1);
      glVertex3f(-1, -1, -1);

    glNormal3f (0.0f, 1.0f, 0.0f);
    //glColor3f(1, 0.1, 0.8);
      glVertex3f( 1,  1,  1);
      glVertex3f( 1,  1, -1);
      glVertex3f(-1,  1, -1);
      glVertex3f(-1,  1,  1);

  glEnd(); // Wir sind fertig mit dem Zeichnen
}

void FlyingQuader () {
  z += 1.0f;
  if (z >= 360.0f) z -= 360.0f;
  if (out) entf -= speed;
  else     entf += speed;
  if (entf <= -100.0f || entf >= -4.0f) out = !out; 

  glTranslatef (0.0f, 0.0f, entf);
  glRotatef(z, 1.0f, 1.0f, 0.0f);
  BuildQuader();
}

void DrawQuad (Texture* tex, float x, float y) {  
    float vertices[] = {
        x, y,
        x, y + tex->height(),
        x + tex->width(), y + tex->height(),
        x + tex->width(), y
    };
    float fac_x = (float)tex->width() / tex->stored_width();
    float fac_y = (float)tex->height() / tex->stored_height();
    float off_x = (float)tex->offset_x() / tex->stored_width();
    float off_y = (float)tex->offset_y() / tex->stored_height();
    float texcoords[] = {
        off_x, off_y,                   // 0, 0
        off_x, fac_y + off_y,           // 0, 1
        fac_x + off_x, fac_y + off_y,   // 1, 1
        fac_x + off_x, off_y            // 1, 0
    };
    
    glBindTexture (GL_TEXTURE_2D, tex->id());
    glVertexPointer (2, GL_FLOAT, 0, vertices);
    glTexCoordPointer (2, GL_FLOAT, 0, texcoords);
    glDrawArrays (GL_QUADS, 0, 4);
}


*/
