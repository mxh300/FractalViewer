# Rutas y configuración
COMPILADOR = C:/raylib/w64devkit/bin/gcc.exe
INCLUDES = -I C:/raylib/raylib/src
LIBRERIAS = -L C:/raylib/raylib/src -lraylib -lgdi32 -lwinmm

# Regla principal
compilar:
	$(COMPILADOR) main.c -o mi_proyecto.exe $(INCLUDES) $(LIBRERIAS)