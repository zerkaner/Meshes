#pragma once

// A computer is like an air conditioner, it stops working when you open Windows.


class IObjectGL {
  public:
    virtual void Render () = 0;
};


/* Bibliotheken, Linkersettings und so:

Die lib-Projekte brauchen nur die VC++ Includes (auf die fremden Pakete).
Die exe-Projekte brauchen 
  - die VC++ Includes (auf die eigenen Projekte, relative Pfade möglich)
  - die VC++ Lib-Verzeichnisse (für die fremden Pakete, die von den eigenen, 
    inkludierten Projekten verwendet werden) 
  - die Linkereinstellungen (->Eingabe), sowohl auf die eigenen (relative Pfade möglich) 
    als auch auf die fremden .lib's
*/

/* Bilder:

- tgas nutzen, non-flipped (original), Achsen beim Converter sind egal
- Rohdaten in Texture-Struct einlesen, in Model3D speichern (Data)
  - ModelGL kann Transformieren in OpenGL-Struktur
*/


/* TODO Wie kriegt man den Scheiß verallgemeinert?

am besten zunächst die button / event geschichte rauspacken, damit das schonmal geht

Ob Terrain, Modell, was auch immer: Direktzeichnung, oder VBO-Unterscheidung soll möglich sein.
- Modus umschalten, strukturen ähnlich (ModelGL und TextureGL?)
  - Texture muß dann nach Data!
- man muß auch alles zeichnen können



*/
