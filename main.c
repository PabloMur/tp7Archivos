#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pila.h"

const int MAYORIA_EDAD = 18;

// Estructura
typedef struct
{
    int legajo;
    char nombreYapellido[30];
    int edad;
    int anio;
} Alumno;


//Funciones Extra
int ComprobarExistenciaArchivo(char nombreArchivo[])
{
    FILE * archivo = fopen(nombreArchivo, "rb");
    int existe = 0;
    if(archivo != NULL) existe = 1;
    return existe;
}

Alumno CrearAlumno()
{
    Alumno al;
    printf("Ingrese legajo: \n");
    scanf("%i", &al.legajo);
    printf("Ingrese nombre completo: \n");
    fflush(stdin);
    scanf("%s", &al.nombreYapellido);
    printf("Ingrese edad: \n");
    fflush(stdin);
    scanf("%i", &al.edad);
    printf("Ingrese año: \n");
    fflush(stdin);
    scanf("%i", &al.anio);
    return al;
}

int contarAlumnosEnArchivo(char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (!archivo) return 0;

    fseek(archivo, 0, SEEK_END);
    long tamanio = ftell(archivo);
    fclose(archivo);

    return tamanio / sizeof(Alumno);
}

// 1.- Agrega elemento al final del archivo
void AgregaElemento(char nombreArchivo[], int elemento)
{
    FILE *archivo = fopen(nombreArchivo, "ab"); // abre en modo binario al final
    if (archivo)
    {
        fwrite(&elemento, sizeof(int), 1, archivo);
        fclose(archivo);
    }
    else
    {
        perror("No se pudo abrir el archivo");
    }
}

// 2.- Mostrar contenido de un archivo
void MostrarContenidoArchivo(char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    int num;

    if (archivo)
    {
        while (fread(&num, sizeof(int), 1, archivo) > 0)
        {
            printf("%i\n", num);
        }
        fclose(archivo);
    }
    else
    {
        printf("No se pudo abrir el archivo.\n");
    }
}

// 3.- Retorna la cantidad de registros
int CantidadElementosArchivo(char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (!archivo)
    {
        printf("No se pudo abrir el archivo.\n");
        return -1;
    }

    fseek(archivo, 0, SEEK_END);
    long tamano = ftell(archivo);
    rewind(archivo);

    int cantidad = tamano / sizeof(int);

    fclose(archivo);
    return cantidad;
}

// 4.- Crear una función que cargue un archivo de alumnos.
//     Abrirlo de manera tal de verificar si el archivo ya
//     está creado previamente. Cargar el archivo con 5 datos.
//     Cerrarlo dentro de la función
void CargarAlumnos(char nombreArchivo[], int cantidadAlumnos)
{
    int existentes = contarAlumnosEnArchivo(nombreArchivo);

    if (existentes == cantidadAlumnos)
    {
        printf("El archivo ya tiene %d alumnos. No se agregan más.\n", existentes);
        return;
    }

    FILE * archivo = fopen(nombreArchivo, "ab");  // se sobrescribe si existía con datos parciales
    if (archivo)
    {
        for(int i = 0; i < cantidadAlumnos; i++)
        {
            Alumno alumno = CrearAlumno();
            fwrite(&alumno, sizeof(Alumno), 1, archivo);
        }
        fclose(archivo);
        printf("Se cargaron %d alumnos correctamente.\n", cantidadAlumnos);
    }
    else
    {
        perror("No se pudo abrir el archivo para escribir alumnos");
    }
}

// 5. Mostrar el listado de alumnos
void MostrarDatosAlumno(Alumno al){
    printf("Leg: %d // Nombre: %s // Edad: %d // Año: %d\n", al.legajo, al.nombreYapellido, al.edad, al.anio);
}

void MostrarAlumnos(char nombreArchivo[])
{
    FILE * arch = fopen(nombreArchivo, "rb");
    Alumno alumno;
    if(arch)
    {
        while(fread(&alumno, sizeof(Alumno), 1, arch) > 0)
        {
            MostrarDatosAlumno(alumno);
        }
    }
    fclose(arch);
}

//6.- Crear una función que permita agregar de a un elemento al final del archivo.
//    O sea, se debe abrir el archivo, se piden los datos (se llena una variable de
//    tipo struct alumno), se escribe en el archivo y se cierra.
void AgregarAlumno(char nombreArchivo[])
{
    FILE * archivo = fopen(nombreArchivo, "ab");
    Alumno al;
    if(archivo)
    {
        al = CrearAlumno();
        fwrite(&al,sizeof(Alumno),1,archivo);
    }
    fclose(archivo);
}

//7.- Crear una función que pase a una pila los números de legajo
//    de los alumnos mayores de edad.
void PasarAPilaLegajos(char nombreArchivo[], Pila * pilaLegajos,int MAYORIA_EDAD)
{
    FILE * archivo = fopen(nombreArchivo, "rb");
    Alumno alumno;

    if(archivo)
    {
        while(fread(&alumno,sizeof(alumno), 1, archivo) > 0)
        {
            if(alumno.edad >= MAYORIA_EDAD)
            {
                apilar(pilaLegajos, alumno.legajo);
            }
        }
    }

    fclose(archivo);
}
//8.- Dado un archivo de alumnos, hacer una función que cuente
//    la cantidad de alumnos mayores a edad específica que se envía por parámetro.
int AlumnosMayoresAUnaEdad(char nombreArchivo[], int edad)
{
    FILE * archivo = fopen(nombreArchivo, "rb");
    int cantidadAlmMayores = 0;
    Alumno al;

    if(archivo)
    {
        while(fread(&al, sizeof(Alumno), 1, archivo) > 0)
        {
            if(al.edad > edad)
            {
                cantidadAlmMayores += 1;
            }
        }
    }

    fclose(archivo);
    return cantidadAlmMayores;
}
//9.- Dado un archivo de alumnos, mostrar por pantalla el nombre de todos
//    los alumnos entre un rango de edades específico (por ejemplo, entre 17 y 25 años).
//    Dicho rango debe recibirse por parámetro. Modularizar
void MostrarNombre(Alumno al){
    printf("Nombre: %s\n", al.nombreYapellido);
}

void AlumnosRangoEdad(char nombreArchivo[], int edadMin, int edadMax){
    FILE * archivo = fopen(nombreArchivo, "rb");
    Alumno al;
    if(archivo){
        while(fread(&al, sizeof(Alumno), 1, archivo)>0){
            if(al.edad > edadMin && al.edad < edadMax) MostrarNombre(al);
        }
    }
    fclose(archivo);
}
//10.- Mostrar alumno con mayor edad


// ==============================
//           MAIN
// ==============================1
int main()
{
    const char *archivoEnteros = "enteros.bin";
    char archivoAlumnos[] = "alumnos.bin";
    int opcion;

    do
    {
        // Limpia la pantalla antes de mostrar el menú
#ifdef _WIN32
        system("cls");    // Windows
#else
        system("clear");  // Linux / macOS
#endif

        printf("\n===================== MENU ================================\n");
        printf("=   1 - Cargar enteros a un archivo.                      =\n");
        printf("=   2 - Mostrar archivo de enteros.                       =\n");
        printf("=   3 - Mostrar cantidad elementos archivo.               =\n");
        printf("=   4 - Cargar Archivo con 5 alumnos.                     =\n");
        printf("=   5 - Mostrar Archivo alumnos.                          =\n");
        printf("=   6 - Agregar un alumno al arch de alumnos.             =\n");
        printf("=   7 - Crear pila de legajos de alumnos mayores.         =\n");
        printf("=   8 - Devuelve cantidad de alumn mayores de una edad.   =\n");
        printf("=   9 - Devuelve nombres de alumno por rango de edad.     =\n");
        printf("=   0 - Salir.                                            =\n");
        printf("\n===========================================================\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
        {
            int num;
            int seguir = 1;
            while (seguir)
            {
                puts("Ingrese un numero entero:");
                fflush(stdin);
                scanf("%i", &num);
                AgregaElemento(archivoEnteros, num);
                printf("Desea continuar?: 1 si / 0 no\n");
                fflush(stdin);
                scanf("%i", &seguir);
            }
            break;
        }

        case 2:
        {
            printf("Contenido del archivo:\n");
            MostrarContenidoArchivo(archivoEnteros);
            printf("Presione Enter para continuar...");
            fflush(stdin);
            getchar();
            getchar();
            break;
        }
        case 3:
        {
            int cantidadElementos = CantidadElementosArchivo(archivoEnteros);
            printf("La cantidad de elementos es: %i\n", cantidadElementos);
            printf("Presione Enter para continuar...");
            fflush(stdin);
            getchar();
            getchar();
            break;
        }
        case 4:
        {
            puts("Cargando Alumnos: \n");
            CargarAlumnos(archivoAlumnos, 5);
            printf("Presione Enter para continuar...");
            fflush(stdin);
            getchar();
            getchar();
            break;
        }
        case 5:
        {
            puts("Listado Alumnos: \n");
            MostrarAlumnos(archivoAlumnos);
            printf("Presione Enter para continuar...");
            fflush(stdin);
            getchar();
            getchar();
            break;
        }
        case 6:
        {
            puts("Agregar alumno: \n");
            AgregarAlumno(archivoAlumnos);
            printf("Presione Enter para continuar...");
            fflush(stdin);
            getchar();
            getchar();
            break;
        }
        case 7:
        {
            Pila pilaLegajos;
            inicpila(&pilaLegajos);
            PasarAPilaLegajos(archivoAlumnos, &pilaLegajos, MAYORIA_EDAD);
            puts("Estudiantes mayores de edad: \n");
            mostrar(&pilaLegajos);
            printf("Presione Enter para continuar...");
            fflush(stdin);
            getchar();
            getchar();
            break;
        }
        case 8:
        {
            int edadRef = 1;
            int alumnosMayoresAEdadPasada = 0;
            puts("Ingrese la edad de referencia: ");
            fflush(stdin);
            scanf("%i",&edadRef);
            alumnosMayoresAEdadPasada = AlumnosMayoresAUnaEdad(archivoAlumnos, edadRef);

            if(alumnosMayoresAEdadPasada)
            {
                printf("La cantidad de alumnos mayores a la edad pasada es de: %d\n", alumnosMayoresAEdadPasada);
            }
            else
            {
                printf("No hay alumnos mayores a la edad pasada.\n");
            }

            printf("Presione Enter para continuar...");
            fflush(stdin);
            getchar();
            getchar();
            break;
        }
        case 9:
        {
            int edadMin, edadMax;
            edadMin = 1;
            edadMax = 120;
            puts("Rango de edades: \n");
            puts("Ingrese la edad minima deseada: \n");
            scanf("%i", &edadMin);
            puts("Ingrese la edad máxima deseada: \n");
            scanf("%i", &edadMax);
            AlumnosRangoEdad(archivoAlumnos, edadMin, edadMax);
            printf("Presione Enter para continuar...");
            fflush(stdin);
            getchar();
            getchar();
            break;
        }
        case 0:
            printf("\n Dale, nos vemos la próxima...\n");
            break;
        default:
            printf("Opción no válida\n");
        }
    }
    while (opcion != 0);

    return 0;
}
