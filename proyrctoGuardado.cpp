#include <string.h>
#include <iostream>
using namespace std;
//LUIS LARA Y GABRIEL FREAY

struct Sventa {
    int num_operacion;
    char producto[50];       
    int cantidad;
    char fecha[15];         
    
    struct Sventa* prventas; 
};

struct Sasociado {
    int codigo;              
    char nombre[50];
    char direccion[100];
    int telefono; 
    
    struct Sasociado* pnext; 
    struct Sventa* pventas;  
};

struct Sproducto {
    int codigo;
    float precio;
    char nombre[30];
    char marca[30];
    char descripcion[100];
    Sproducto* psig;
};

bool BuscarCodigoProducto(Sproducto *lista, int codigo){
    if(!(lista)){
        return true;
    }
    else{
        while(lista){
            if(lista->codigo==codigo){
                return false;
            }
            else{lista=lista->psig;}
        }
        return true;
    }
}

bool BuscarCodigoAsociado(Sasociado *lista, int codigo){
    if(!(lista)){
        return true;
    }
    else{
        while(lista){
            if(lista->codigo==codigo){
                return false;
            }
            else{lista=lista->pnext;}
        }
        return true;
    }
}

void ToLower(const char origen[], char destino[]) {
    size_t i = 0;
    while (origen[i] != '\0') {
        char c = origen[i];
        if (c >= 'A' && c <= 'Z') {
            destino[i] = c - 'A' + 'a';
        } else {
            destino[i] = c;
        }
        ++i;
    }
    destino[i] = '\0';
}

static int ContainsIgnoreCase(const char texto[], const char patron[]) {
    char textoMinusculas[200];
    char patronMinusculas[200];
    ToLower(texto, textoMinusculas);
    ToLower(patron, patronMinusculas);
    return strstr(textoMinusculas, patronMinusculas) != NULL;
}

void GuardarProductos(Sproducto *lista){
	FILE *f = fopen("productos.txt", "w");
	if(!f) { 
		printf("Error al abrir el archivo .txt \n");
		return;
	}
	while(lista) {
		fprintf(f, "%d|%s|%s|%s\n",
			lista->codigo,
			lista->nombre,
			lista->marca,
			lista->descripcion);
		lista=lista->psig;
	}
	fclose(f);
}

void CargarProductos(Sproducto **lista) {
	FILE *f = fopen("productos.txt", "r");
	if(!f) {return;}
	while(1) {
		Sproducto *nuevo = new Sproducto;
		if (fscanf(f, "%d|%29[^|]|%29[^|]|%99[^\n]\n",
			&nuevo->codigo,
			nuevo->nombre,
			nuevo->marca,
			nuevo->descripcion) == 4) {
			nuevo->psig = *lista;
			*lista = nuevo;
		}
		else{
			delete nuevo;
			break;
		}
	}
	fclose(f);
}

void GuardarAsociados(Sasociado *lista){
	FILE *f = fopen("asociados.txt", "w");
	if (!f) {
	printf("error al abrir asociados.txt\n");
	return;
	}
	while (lista){
	fprintf(f,  "%d|%s|%s|%d\n",
		lista->codigo,
		lista->nombre,
		lista->direccion,
		lista->telefono);
	lista = lista->pnext;
	}
	fclose(f);
}

void CargarAsociados(Sasociado **lista) {
    FILE *f = fopen("asociados.txt", "r");
    if (!f) return;
    Sasociado *nuevo;
    while (1) {
        nuevo = new Sasociado;
        if (fscanf(f, "%d|%49[^|]|%99[^|]|%d\n",
            &nuevo->codigo,
            nuevo->nombre,
            nuevo->direccion,
            &nuevo->telefono) == 4) {
            nuevo->pventas = NULL;
            nuevo->pnext = *lista;
            *lista = nuevo;
        } else {
            delete nuevo;
            break;
        }
    }
    fclose(f);
}

//============================ASOCIADOS============================
void AgregarAsociados(Sasociado **lista, Sasociado** asociado){
        (*asociado)->pnext=*lista;
        *lista=*asociado;
        printf("Codigo agregado con exito!");
}

Sasociado* NuevoAsociado(Sasociado *ListaAsociados){
    Sasociado *nuevo=new Sasociado;
    char nombre[100];
    char direccion[150];
    int telefono;
    int codigo;
	int valido=0;
    
    printf("Ingrese el nombre del Asociado (No puede ser mayor de 30 caracteres): \n");
    fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
    while(strlen(nombre)>sizeof(nuevo->nombre)){
        printf("El nombre excede los 30 caracteres, por favor introduzca uno mas corto");
        fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
    }
    strcpy(nuevo->nombre, nombre);

    printf("Ingrese la direcccion del Asociado (No puede ser mayor de 100 caracteres): \n");
    fflush(stdin);fflush(stdin);scanf(" %149[^\n]", direccion);fflush(stdin);
    while(strlen(direccion)>sizeof(nuevo->direccion)){
        printf("La direccion excede los 100 caracteres, por favor introduzca uno mas corto");
        fflush(stdin);fflush(stdin);scanf(" %149[^\n]", direccion);fflush(stdin);
    }
    strcpy(nuevo->direccion, direccion);

    printf("Ingrese el numero de telefono del Asociado: ");
    fflush(stdin);scanf("%d", &telefono);fflush(stdin);
    nuevo->telefono=telefono;

    printf("Ingrese el codigo del Asociado: ");
    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
	while(!valido){
		if(scanf("%d", &codigo)==1){
			valido=1;
		}
		else{
			printf("Eso no es un entero, intente nuevamente:");
			scanf("%d", &codigo);fflush(stdin);
		}
	}

    while(!(BuscarCodigoAsociado(ListaAsociados, codigo))){
        printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
        fflush(stdin);scanf("%d", &codigo);fflush(stdin);
    }
    nuevo->codigo=codigo;

    nuevo->pnext=NULL;
    nuevo->pventas=NULL;
    return nuevo;
}

void MostrarAsociado(Sasociado *p){
    if(!p) return;
    printf("Nombre: %s | Direccion: %s | Numero de telefono: %d | Codigo del producto: %d\n", 
    p->nombre, p->direccion, p->telefono, p->codigo);
}

void ConsultarCodigoAsociado(Sasociado *lista, int n){
    if(!(lista)){
        printf("No hay asociados en la lista\n");
    }

    else{
        Sasociado *aux = lista;
        while(aux && aux->codigo != n){
            aux = aux->pnext;
        }

        if(aux && aux->codigo == n){
            MostrarAsociado(aux);
        }
        else{
            printf("El codigo no esta en la lista\n");
        }
    }
}

void BuscarPorNombreAsociado(Sasociado *lista, const char nombre[]){
    if(!lista){
        printf("No hay productos en la lista\n");
        return;
    }

    bool encontrado = false;
    while(lista){
        if(ContainsIgnoreCase(lista->nombre, nombre)){
            MostrarAsociado(lista);
            encontrado = true;
        }
        lista = lista->pnext;
    }

    if(!encontrado){
        printf("No se encontro ningun producto con ese nombre\n");
    }
}

Sasociado* ExisteAsociado(Sasociado *lista, int n){
    Sasociado* ax = lista;
    while(ax && ax->codigo != n){
        ax = ax->pnext;
    }
    return ax;
}

void ModificarCodigoAsociado(Sasociado *lista){
    int CambioNum;
    int menu;
    char Cambio[150];

    if(lista==NULL){
        printf("La lista esta vacia. ");
        return;
    }
    else{
    printf("Que deseas modificar: \n");
    printf("1- Nombre. \n");
    printf("2- Direccion. \n");
    printf("3- Telefono. \n");
    printf("4- Codigo. \n");
    fflush(stdin);scanf("%d", &menu);fflush(stdin);
    switch(menu){
        case 1:
            printf("Ingrese el nombre: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(lista->nombre)){
                printf("El nombre excede los 30 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
                }
            strcpy(lista->nombre, Cambio);
            fflush(stdin);
            printf("El codigo fue cambiado con exito!");
            break;
        case 2:
            printf("Ingrese la direccion: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(lista->direccion)){
                printf("La direccion se excede los 100 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
            }
            strcpy(lista->direccion, Cambio);
            fflush(stdin);
            printf("El codigo fue cambiado con exito!");
            break;
        case 3:
            printf("Ingrese el numero de telefono: \n");
            scanf(" %d", &CambioNum);fflush(stdin);
            lista->telefono=CambioNum;
            fflush(stdin);
            printf("El codigo fue cambiado con exito!");
            break;
        case 4:
            printf("Ingrese el codigo del Asociado: ");
            scanf("%d", &CambioNum);fflush(stdin);
            while(!(BuscarCodigoAsociado(lista, CambioNum))){
                printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
                fflush(stdin);scanf("%d", &CambioNum);fflush(stdin);
            }
            lista->codigo=CambioNum;
            break;
        default:
            printf("Por favor introduzca una opcion valida. \n");
            fflush(stdin);scanf("%d", &menu);fflush(stdin);
        }
    }
}

void EliminarPorCodigoAsociado(Sasociado **a, int n){
    Sasociado *aux;
    Sasociado *t;
    if((*a)->codigo==n){
        aux=*a;
        *a=(*a)->pnext;
        delete aux;
    }
    else{
        aux=*a;
        while(aux->pnext && aux->pnext->codigo!=n){
            aux=aux->pnext;
        }
            if(aux->pnext!=NULL){
                t=aux->pnext;
                aux->pnext=t->pnext;
                delete t;
                printf("El codigo fue eliminado con exito!");
            }
            else{
                printf("El codigo no esta en la lista");
            }
    }
}

void MostrarAsociados(Sasociado *lista){
    while(lista){
        printf("Nombre: %s | Direccion: %s | Numero de telefono: %d | Codigo del producto: %d\n", 
        lista->nombre, lista->direccion, lista->telefono, lista->codigo);
        lista=lista->pnext;
    }
}
//============================PRODUCTO============================
Sproducto* NuevoProducto(Sproducto *ListaProductos){ //AHORA EL NUEVO PARAMETRO TIENE QUE SER LA LISTA DE LOS PRODUCTOS
    Sproducto *nuevo=new Sproducto;
    char temp[100];
    char marca[100];
    char descripcion[150];
    int codigo;
	int entrada_valida;

    //Abro el archivo
    printf("Ingrese el nombre (No puede ser mayor de 30 caracteres): \n");
    fflush(stdin);fflush(stdin);scanf(" %99[^\n]", temp);  fflush(stdin); //Importante el fflush asi limpio buffer
    while (strlen(temp)>=sizeof(nuevo->nombre)){
        printf("El nombre excede los 30 caracteres, por favor introduzca algo mas corto. \n");
        fflush(stdin);scanf(" %99[^\n]", temp);
    }
    strcpy(nuevo->nombre, temp);

    printf("Ingrese la marca (No puede ser mayor de 30 caracteres): \n");
    fflush(stdin);scanf(" %99[^\n]", marca);fflush(stdin);
    while(strlen(marca)>=sizeof(nuevo->marca)){
        printf("Muy largo. \n");
        fflush(stdin);scanf(" %99[^\n]", marca);fflush(stdin);
    }
    strcpy(nuevo->marca, marca);

    printf("Ingrese el descripcion (No puede ser mayor a 100 caracteres): \n");
    fflush(stdin);scanf(" %99[^\n]", descripcion);fflush(stdin);
    while(strlen(descripcion)>=sizeof(nuevo->descripcion)){
        printf("Muy largo. \n");
        fflush(stdin);scanf(" %99[^\n]", descripcion);fflush(stdin);
    }
    strcpy(nuevo->descripcion, descripcion);

    printf("Ingrese el codigo del producto: ");
    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
    while(!(BuscarCodigoProducto(ListaProductos, codigo))){
        printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
        fflush(stdin);scanf("%d", &codigo);fflush(stdin);
    }
    nuevo->codigo=codigo;
    nuevo->psig=NULL;
    return nuevo;
    //Cierro archivo (Asi en todas las funciones)
}

void AgregarProductos(Sproducto **lista, Sproducto** producto){
        (*producto)->psig=*lista;
        *lista=*producto;
        printf("Codigo agregado con exito!");
}

void MostrarProductos(Sproducto *lista){
    while(lista){
        printf("Nombre: %s | Nombre de la marca: %s | Descripcion del producto: %s | Codigo del producto: %d\n", 
        lista->nombre, lista->marca, lista->descripcion, lista->codigo);
        lista=lista->psig;
    }
}

void MostrarProducto(Sproducto *p){
    if(!p) return;
        printf("Nombre: %s | Nombre de la marca: %s | Descripcion del producto: %s | Codigo del producto: %d\n", 
        p->nombre, p->marca, p->descripcion, p->codigo);
}

void ConsultarCodigo(Sproducto *lista, int n){
    if(!(lista)){
        printf("No hay productos en la lista\n");
    }

    else{
        Sproducto *aux = lista;
        while(aux && aux->codigo != n){
            aux = aux->psig;
        }

        if(aux && aux->codigo == n){
            MostrarProducto(aux);
        }
        else{
            printf("El codigo no esta en la lista\n");
        }
    }
}

void BuscarPorNombre(Sproducto *lista, const char nombre[]){
    if(!lista){
        printf("No hay productos en la lista\n");
        return;
    }

    bool encontrado = false;
    while(lista){
        if(ContainsIgnoreCase(lista->nombre, nombre)){
            MostrarProducto(lista);
            encontrado = true;
        }
        lista = lista->psig;
    }

    if(!encontrado){
        printf("No se encontro ningun producto con ese nombre\n");
    }
}

Sproducto* ExisteProducto(Sproducto *lista, int n){
    Sproducto* ax = lista;
    while(ax && ax->codigo != n){
        ax = ax->psig;
    }
    return ax;
}

void ModificarCodigo(Sproducto *lista){
    int mod;
    int menu;
    char Cambio[150];

    if(lista==NULL){
        printf("La lista esta vacia. ");
        return;
    }
    else{
    printf("Que deseas modificar: \n");
    printf("1- Nombre. \n");
    printf("2- Marca. \n");
    printf("3- Descripcion. \n");
    printf("4- Precio. \n");
    printf("5- Codigo. \n");
    fflush(stdin);scanf("%d", &menu);fflush(stdin);
    switch(menu){
        case 1:
            printf("Ingrese el nombre: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(lista->nombre)){
                printf("El nombre excede los 30 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
                }
            strcpy(lista->nombre, Cambio);
            fflush(stdin);
            printf("El codigo fue cambiado con exito!");
            break;
        case 2:
            printf("Ingrese la direccion: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(lista->marca)){
                printf("La marca se excede los 30 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
            }
            strcpy(lista->marca, Cambio);
            fflush(stdin);
            printf("El codigo fue cambiado con exito!");
            break;
        case 3:
            printf("Ingrese la descripcion: \n");
            scanf(" %149[^\n]", Cambio);fflush(stdin);
            while (strlen(Cambio)>=sizeof(lista->descripcion)){
                printf("La descripcion se excede los 100 caracteres, por favor introduzca algo mas corto. \n");
                fflush(stdin);scanf(" %99[^\n]", Cambio);
                }
            strcpy(lista->descripcion, Cambio);
            fflush(stdin);
            printf("El codigo fue cambiado con exito!");
            break;
        case 4:
            printf("Ingrese el precio: \n");
            fflush(stdin);scanf("%f", &mod);fflush(stdin);
            while(mod<=0){
                printf("Por favor introduzca un numero mayor a 0: \n");
                fflush(stdin);scanf("%f", &mod);fflush(stdin);
            }
            lista->precio=mod;
        //Agregar case 5 modificar codigo
            break;
        case 5:
            printf("Ingrese el codigo del Asociado: ");
            scanf("%d", &mod);fflush(stdin);
            while(!(BuscarCodigoProducto(lista, mod))){
                printf("El codigo ya se encuentra en la lista, por favor introduzca uno distinto: ");
                fflush(stdin);scanf("%d", &mod);fflush(stdin);
            }
            lista->codigo=mod;
            break;
        default:
            printf("Por favor introduzca una opcion valida. \n");
            fflush(stdin);scanf("%d", &menu);fflush(stdin);
            }
        }
    }

void EliminarPorCodigo(Sproducto **a, int n){
    Sproducto *aux;
    Sproducto *t;
    if((*a)->codigo==n){
        aux=*a;
        *a=(*a)->psig;
        delete aux;
    }
    else{
        aux=*a;
        while(aux->psig && aux->psig->codigo!=n){
            aux=aux->psig;
        }
            if(aux->psig!=NULL){
                t=aux->psig;
                aux->psig=t->psig;
                delete t;
                printf("El codigo fue cambiado con exito!");
            }
            else{
                printf("El codigo no esta en la lista");
            }
    }
}


int main(){
    int menu=1;
    int option=1;
    int codigo;
    int CodigoAsociados;
    Sproducto* ListaProductos=NULL;
    Sasociado* ListaAsociados=NULL;
	Sproducto* prueba;
    Sasociado* pruebasociado;
	CargarProductos(&ListaProductos);
	CargarAsociados(&ListaAsociados);
    while (menu!=0){  
    printf("\n\n\t\tSistema de ventas DirVen\n\n");  
    printf("1. Asociados \n");  
    printf("2. Productos \n"); 
    printf("3. Ventas \n");  
    printf("\n0. Salir\n");  
    fflush(stdin);scanf("%d", &menu);fflush(stdin);  
    switch (menu){
        //ASOCIADOS
        case 1: 
        while (option!=0){
        printf("\n\n1.1.1.1 Agregar \n"); // CAMBIAR
        printf("1.1.1.2 Consultar por codigo \n");
        printf("1.1.1.3 Consultar por nombre \n");
        printf("1.1.1.4 Modificar por codigo \n");
        printf("1.1.1.5 Eliminar por codigo \n");
        printf("1.1.1.6 Mostrar todos los productos \n");
        printf("1.1.1.0 Salir \n");
        //======================================MENU ASOCIADOS======================================
        fflush(stdin);scanf("%d", &option);fflush(stdin);
            switch(option){
                case 1: {
                    Sasociado* NewAsociado=NuevoAsociado(ListaAsociados);
                    AgregarAsociados(&ListaAsociados, &NewAsociado);
					GuardarAsociados(ListaAsociados);
                    break;
                }
                case 2: {
                    printf("Introduzca el codigo que desea consultar: ");
                    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
                    ConsultarCodigoAsociado(ListaAsociados, codigo);
                    break;
                }
                case 3: {
                    char nombre[100];
                    printf("Introduzca el nombre del asociado que desea consultar: \n");
                    fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
                    while(strlen(nombre)>sizeof(ListaAsociados->nombre)){
                        printf("El nombre excede los 30 caracteres, por favor introduzca uno mas corto");
                        fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
                    }
                    BuscarPorNombreAsociado(ListaAsociados, nombre);
                    break;
                }
                case 4: {
                    printf("Que codigo deseas modificar: ");
                    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
                    pruebasociado=ExisteAsociado(ListaAsociados, codigo);
                    if(pruebasociado != NULL){
                        ModificarCodigoAsociado(pruebasociado);
                    } 
                    else {
                        printf("No se encontro el producto con ese codigo.\n");
                    }
                    break;
                }
                case 5: {
                    printf("Que codigo desea eliminar?: ");
                    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
                    EliminarPorCodigoAsociado(&ListaAsociados, codigo);
					GuardarAsociados(ListaAsociados);
                    break;
                }
                case 6: {
                    MostrarAsociados(ListaAsociados);
                    break;
                }
            }
        }
        option=-1;
        break;
        //PRODUCTOS
        case 2: 
        while (option!=0){
        printf("\n\n1.1.1.1 Agregar \n"); //LISTO
        printf("1.1.1.2 Consultar por codigo \n"); //LISTO
        printf("1.1.1.3 Consultar por nombre \n"); 
        printf("1.1.1.4 Modificar por codigo \n"); //LISTO
        printf("1.1.1.5 Eliminar por codigo \n");  //LISTO
        printf("1.1.1.6 Mostrar todos los productos \n"); //LISTO
        printf("1.1.1.0 Salir \n");
        fflush(stdin);scanf("%d", &option);fflush(stdin);
            switch(option){
                case 0:{
                    break;
                }
                case 1: {
                    Sproducto* NewProducto=NuevoProducto(ListaProductos);
                    AgregarProductos(&ListaProductos, &NewProducto);
					GuardarProductos(ListaProductos);
                    break;
                }

                case 2: {
                    printf("Introduzca el codigo que desea consultar: ");
                    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
                    ConsultarCodigo(ListaProductos, codigo);
                    break;
                }

                case 3: {
                    char nombre[100];
                    printf("Introduzca el nombre del producto que desea consultar: \n");
                    fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
                    while(strlen(nombre)>sizeof(ListaProductos->nombre)){
                        printf("El nombre excede los 30 caracteres, por favor introduzca uno mas corto");
                        fflush(stdin);fflush(stdin);scanf(" %99[^\n]", nombre);fflush(stdin);
                    }
                    BuscarPorNombre(ListaProductos, nombre);
                    break;
                }

                case 4: {
                    printf("Que codigo deseas modificar");
                    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
                    prueba=ExisteProducto(ListaProductos, codigo);
                    prueba = ExisteProducto(ListaProductos, codigo);
                    if(prueba != NULL){
                        ModificarCodigo(prueba);
                    } 
                    else {
                        printf("No se encontro el producto con ese codigo.\n");
                    }
                    ModificarCodigo(prueba);
                    break;
                    
                }
                
                case 5: {
                    printf("Que codigo desea eliminar?: ");
                    fflush(stdin);scanf("%d", &codigo);fflush(stdin);
                    EliminarPorCodigo(&ListaProductos, codigo);
					GuardarProductos(ListaProductos);
                    break;
                }

                case 6: {
                    MostrarProductos(ListaProductos);
                    break;
                }
            }
        }
        option=-1;
        break;
    
    }
    }
    return 0;
}
//Output, agrego la libreria de archivo.
//funciones de validacion.
//std:ofstrean archivo(Nombre del archivo)
//if nombre.is_open()
//archivo<<
//archivo.close