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
    int c;

    printf("Ingrese legajo: \n");
    scanf("%i", &al.legajo);

    // Limpiar el buffer de entrada
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Ingrese nombre completo: \n");
    fgets(al.nombreYapellido, sizeof(al.nombreYapellido), stdin);
    // Eliminar salto de línea al final si existe
    al.nombreYapellido[strcspn(al.nombreYapellido, "\n")] = '\0';

    printf("Ingrese edad: \n");
    scanf("%i", &al.edad);

    printf("Ingrese año: \n");
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
void MostrarDatosAlumno(Alumno al)
{
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
        fclose(arch);
    }
}
void MostrarAlumnosXLegajo(char nombreArchivo[], int legajoAlumno)
{
    FILE * arch = fopen(nombreArchivo, "rb");
    Alumno al;
    if(arch)
    {
        while(fread(&al, sizeof(Alumno), 1, arch)>0)
        {
            if(al.legajo == legajoAlumno)
            {
                MostrarDatosAlumno(al);
            }
        }
        fclose(arch);
    }
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
void MostrarNombre(Alumno al)
{
    printf("Nombre: %s\n", al.nombreYapellido);
}

void AlumnosRangoEdad(char nombreArchivo[], int edadMin, int edadMax)
{
    FILE * archivo = fopen(nombreArchivo, "rb");
    Alumno al;
    if(archivo)
    {
        while(fread(&al, sizeof(Alumno), 1, archivo)>0)
        {
            if(al.edad > edadMin && al.edad < edadMax) MostrarNombre(al);
        }
    }
    fclose(archivo);
}
//10.- Mostrar alumno con mayor edad
int LegajoDelMayor(char nombreArchivo[])
{
    FILE * arch = fopen(nombreArchivo, "rb");
    int mayor = 1;
    int legajo = 0;
    Alumno al;

    if(arch)
    {
        while(fread(&al, sizeof(Alumno), 1, arch) > 0)
        {
            if(al.edad > mayor)
            {
                mayor = al.edad;
                legajo = al.legajo;
            }
        }
        fclose(arch);
    }
    return legajo;
}

// 11.- Mostrar listado de alumnos por un anio especifico
int CantidadAlumnosXAnio(char nombreArchivo[], int anio)
{
    FILE * archivo = fopen(nombreArchivo, "rb");
    Alumno al;
    int cantidadAlumnos = 0;
    if(archivo)
    {
        while(fread(&al, sizeof(Alumno), 1, archivo)> 0)
        {
            if(al.anio == anio)
            {
                cantidadAlumnos+=1;
            }
        }
        fclose(archivo);
    }
    return cantidadAlumnos;
}
//12.- Crear una función que reciba como parámetro un arreglo de tipo
//     alumno y lo copie en el archivo. Asimismo, realice otra función
//     que pase los elementos del archivo a un arreglo de alumnos, filtrando
//     los estudiantes de un año en particular.
void PasarAlumnosDeArregloALArchivo(char nombreArchivo[], Alumno curso[], int validos)
{
    FILE * archivo = fopen(nombreArchivo, "ab");
    if(archivo)
    {
        for (int i = 0; i < validos; i++)
        {
            fwrite(&curso[i], sizeof(Alumno), 1, archivo);
        }
        fclose(archivo);
    }
}

int PasarAlumnosDeArchivoAArreglo(char nombreArchivo[], Alumno curso[], int max, int anio)
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    Alumno al;
    int i = 0;

    if (archivo)
    {
        while (fread(&al, sizeof(Alumno), 1, archivo) > 0 && i < max)
        {
            if (al.anio == anio)
            {
                curso[i] = al;
                i++;
            }
        }
        fclose(archivo);
    }

    return i; // devuelve la cantidad de alumnos válidos
}


void opcionPasarAlumnosPorAnio(char nombreArchivo[])
{
#define MAX_ALUMNOS 100
    Alumno alumnos[MAX_ALUMNOS];
    int anio;

    printf("Ingrese el año de los alumnos a buscar: ");
    scanf("%d", &anio);

    int cantidad = PasarAlumnosDeArchivoAArreglo(nombreArchivo, alumnos, MAX_ALUMNOS, anio);

    printf("Se encontraron %d alumno(s) del año %d:\n", cantidad, anio);
    for (int i = 0; i < cantidad; i++)
    {
        printf("- %s %s\n", alumnos[i].nombreYapellido);
    }
}
// 14. Crear una función que retorne la cantidad de registros que
//     tiene el archivo. Usar fseek y ftell. Puede pensar la función
//     para uso genérico, que sirva para averiguar la cantidad de registros
//     de cualquier archivo.

int RetornaCantidadRegistros(char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "rb");  // Abrimos en modo binario
    if (archivo == NULL)
    {
        perror("No se pudo abrir el archivo");
        return -1;
    }

    // Ir al final del archivo
    fseek(archivo, 0, SEEK_END);
    long tamanio_archivo = ftell(archivo);
    fclose(archivo);

    if (tamanio_archivo < 0)
    {
        return -1;
    }

    // Calcular la cantidad de registros
    return tamanio_archivo / sizeof(Alumno);
}

//15.- Dado un archivo de alumnos, que tenga al menos 10 registros,
//     hacer una función que muestre el contenido de un registro,
//     cuyo número (entre 0 y 9) se pase por parámetro. Controlar no
//     sobrepasar los límites del archivo.
void MostrarInfoAlumnoXPosicion(char nombreArchivo[], int indice)
{
    FILE * archivo = fopen(nombreArchivo,"rb");
    Alumno al;
    int counter = 0;

    if(archivo)
    {
        while(fread(&al, sizeof(Alumno), 1, archivo) > 0)
        {
            if(indice == counter)
            {
                MostrarDatosAlumno(al);
                fclose(archivo);
                return;
            }
            counter++;
        }
        printf("No se encontró el alumno en la posición indicada.\n");
        fclose(archivo);
    }
    else
    {
        printf("No se pudo abrir el archivo.\n");
    }
}
//16.- Realice una (o varias) funciones que permitan modificar un registro
//     existente en el archivo de alumnos. La misma debe permitir modificar
//     uno o todos los campos de la estructura y sobreescribir el registro
//     existente en el archivo.
void ModificarDatosAlumno(char nombreArchivo[]){
    FILE * archivo = fopen(nombreArchivo, "rb");
    if(archivo){
        //  ver como era la onda aca
    }

    MostrarAlumnos();
}

//17.- Dado un archivo de alumnos, hacer una función que invierta los elementos
//     del mismo. No se puede usar otro archivo auxiliar ni un arreglo auxiliar.
//     Debe trabajar sobre el archivo. Puede utilizar variables de tipo alumno auxiliares.

void InvertirArchivoAlumnos(char nombreArchivo[]) {
    FILE *archivo = fopen(nombreArchivo, "r+b");
    if (!archivo) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    int cantidad = RetornaCantidadRegistros(nombreArchivo);
    Alumno a1, a2;

    for (int i = 0; i < cantidad / 2; i++) {
        // Leer alumno en la posición i
        fseek(archivo, i * sizeof(Alumno), SEEK_SET);
        fread(&a1, sizeof(Alumno), 1, archivo);

        // Leer alumno en la posición (cantidad - 1 - i)
        fseek(archivo, (cantidad - 1 - i) * sizeof(Alumno), SEEK_SET);
        fread(&a2, sizeof(Alumno), 1, archivo);

        // Escribir a2 en la posición i
        fseek(archivo, i * sizeof(Alumno), SEEK_SET);
        fwrite(&a2, sizeof(Alumno), 1, archivo);

        // Escribir a1 en la posición (cantidad - 1 - i)
        fseek(archivo, (cantidad - 1 - i) * sizeof(Alumno), SEEK_SET);
        fwrite(&a1, sizeof(Alumno), 1, archivo);
    }

    fclose(archivo);
}


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

        printf("\n===================== MENU =================================\n");
        printf("=   1  - Cargar enteros a un archivo.                       =\n");
        printf("=   2  - Mostrar archivo de enteros.                        =\n");
        printf("=   3  - Mostrar cantidad elementos archivo.                =\n");
        printf("=   4  - Cargar Archivo con 5 alumnos.                      =\n");
        printf("=   5  - Mostrar Archivo alumnos.                           =\n");
        printf("=   6  - Agregar un alumno al arch de alumnos.              =\n");
        printf("=   7  - Crear pila de legajos de alumnos mayores.          =\n");
        printf("=   8  - Devuelve cantidad de alumn mayores de una edad.    =\n");
        printf("=   9  - Devuelve nombres de alumno por rango de edad.      =\n");
        printf("=   10 - Datos del alumno de mayor edad.                    =\n");
        printf("=   11 - Cantidad de alumnos por anio.                      =\n");
        printf("=   12 - Pasar alumnos de array a archivo.                  =\n");
        printf("=   13 - Pasar alumnos de archivo a arreglo por año.        =\n");
        printf("=   14 - Retorna cantidad de registros en archivo.          =\n");
        printf("=   15 - Mostrar datos de alumno por indice.                =\n");
        printf("=   0  - Salir.                                             =\n");
        printf("\n============================================================\n");
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
        case 10:
        {
            int mayor = LegajoDelMayor(archivoAlumnos);
            MostrarAlumnosXLegajo(archivoAlumnos, mayor);
            printf("Presione Enter para continuar...");
            fflush(stdin);
            getchar();
            getchar();
            break;
        }
        case 11:
        {
            int anio = 1;
            puts("Ingrese el anio del curso buscado: \n");
            fflush(stdin);
            scanf("%i",&anio);
            int cantidadAlms = CantidadAlumnosXAnio(archivoAlumnos, anio);
            printf("La cantidad de alumnos en ese anio es de: %d\n", cantidadAlms);
            printf("Presione Enter para continuar...");
            fflush(stdin);
            getchar();
            getchar();
            break;
        }
        case 12:
        {
            Alumno curso[5] =
            {
                {1001, "Juan Perez", 2022},
                {1002, "Ana Garcia", 2023},
                {1003, "Luis Martinez", 2022},
                {1004, "Sofia López", 2024},
                {1005, "Carlos Diaz", 2023}
            };
            PasarAlumnosDeArregloALArchivo(archivoAlumnos, curso, 5);
            printf("Presione Enter para continuar...");
            fflush(stdin);
            getchar();
            getchar();
            break;
        }
        case 13:
        {
            opcionPasarAlumnosPorAnio(archivoAlumnos);
            printf("Presione Enter para continuar...");
            fflush(stdin);
            getchar();
            getchar();
            break;
        }
        case 14:
        {
            int opcionArchivo;
            puts("Vamos a ver la cantidad de elementos que tiene un archivo.\n");
            puts("Hasta el momento tenemos dos archivos: .\n");
            puts("1- Archivo Alumnos.\n");
            puts("2- Archivo Enteros.\n");
            puts("Ingrese la opcion que desea ver: \n");
            scanf("%i", &opcionArchivo);
            switch(opcionArchivo)
            {
            case 1:
            {
                int cantidadReg = RetornaCantidadRegistros(archivoAlumnos);
                printf("La cantidad de registros en el archivo de alumnos es de: %i\n", cantidadReg);
                break;
            }
            case 2:
            {
                int cantidadReg = RetornaCantidadRegistros(archivoEnteros);
                printf("La cantidad de registros en el archivo de enteros es de: %i\n", cantidadReg);
                break;
            }
            default:
            {
                printf("no se ingreso una opcion válida.\n");
                break;
            }
            }

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
