/*
AUTOR: LEONARDO FAVIO HERNANDEZ PEÑARANDA 
SEGUNDO SEMESTRE
INGENIERIA EN SISTEMAS
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>
using namespace std;

struct comprador {
    int idUsuario;
    string nombre, correo, clave, direccion, pago;
    bool bono=false;
};

struct producto {
    int idProducto;
    string nombre, descripcion;
    double precio;
    int stock;
};

struct comentarios {
    int idComentario;
    string producto;
    string usuario;
    string texto;
    string fecha;
};

struct carritocompras {
    int idCarrito;
    int idUsuarioDuenio;
    string nombreProducto;
    double precioProducto;
};

struct ordenDeCompra {
    int iDorden=0;
    double subtotal=0;
    double impuestos=0;
    double envio=20000;
    double descuento=0;
    double total=0;
};

vector<comprador> listaUsuarios;
vector<producto> listaProductos;
vector<comentarios> listaComentarios;
vector<carritocompras> listaCarrito;
vector<ordenDeCompra> ordenes;
vector<carritocompras> historialVentasGlobales;

void cargarUsuarios(){
    ifstream archivo("Usuarios.txt");
    string linea, basura;
    getline(archivo, basura);
    while(getline(archivo, linea)){
        stringstream ss(linea);
        string sId, nom, corr, cla, dir, pag;
        getline(ss, sId, ','); getline(ss, nom, ','); getline(ss, corr, ',');
        getline(ss, cla, ','); getline(ss, dir, ','); getline(ss, pag, ',');
        comprador temp;
        temp.idUsuario=atoi(sId.c_str());
        temp.nombre=nom; temp.correo=corr; temp.clave=cla;
        temp.direccion=dir; temp.pago=pag;
        listaUsuarios.push_back(temp);
    }
    archivo.close();
}

void cargarProductos(){
    ifstream archivo("Productos.txt");
    string linea, basura;
    getline(archivo, basura);
    while(getline(archivo, linea)){
        stringstream ss(linea);
        string sId, nom, des, sPre, sSto;
        getline(ss, sId, ','); getline(ss, nom, ','); getline(ss, des, ',');
        getline(ss, sPre, ','); getline(ss, sSto, ',');
        producto temp;
        temp.idProducto=atoi(sId.c_str());
        temp.nombre=nom; temp.descripcion=des;
        temp.precio=atof(sPre.c_str());
        temp.stock=atoi(sSto.c_str());
        listaProductos.push_back(temp);
    }
    archivo.close();
}

void cargarComentarios(){
    ifstream archivo("Comentarios.txt");
    string linea, basura;
    getline(archivo, basura);
    while(getline(archivo, linea)){
        stringstream ss(linea);
        string sId, prod, usu, txt, fec;
        getline(ss, sId, ','); getline(ss, prod, ','); getline(ss, usu, ',');
        getline(ss, txt, ','); getline(ss, fec, ',');
        comentarios temp;
        temp.idComentario=atoi(sId.c_str());
        temp.producto=prod;
        temp.usuario=usu;
        temp.texto=txt;
        temp.fecha=fec;
        listaComentarios.push_back(temp);
    }
    archivo.close();
}

void cargarCarrito(){
    ifstream archivo("Carrito.txt");
    if(!archivo.is_open()) return;
    string linea, basura;
    getline(archivo, basura);
    while(getline(archivo, linea)){
        stringstream ss(linea);
        string sId, sIdU, nom, sPre;
        getline(ss, sId, ','); getline(ss, sIdU, ',');
        getline(ss, nom, ','); getline(ss, sPre, ',');
        carritocompras temp;
        temp.idCarrito=atoi(sId.c_str());
        temp.idUsuarioDuenio=atoi(sIdU.c_str());
        temp.nombreProducto=nom;
        temp.precioProducto=atof(sPre.c_str());
        listaCarrito.push_back(temp);
    }
    archivo.close();
}

int login(){
    string c, p;
    cout<<"\n--- LOGIN ---"<<endl;
    cout<<"Correo: "; cin>>c;
    cout<<"Clave: "; cin>>p;
    for(int i=0;i<listaUsuarios.size();i++){
        if(listaUsuarios[i].correo==c && listaUsuarios[i].clave==p) return i;
    }
    return -1;
}

void mostrarUsuarios(){
    cout<<"\n--- LISTA DE USUARIOS ---"<<endl;
    for(int i=0;i<listaUsuarios.size();i++){
        string nomM=listaUsuarios[i].nombre;
        for(int j=0;j<nomM.length();j++) nomM[j]=toupper(nomM[j]);
        cout<<"Nombre: "<<nomM<<" | Correo: "<<listaUsuarios[i].correo<<endl;
    }
}

void mostrarStockBajo(){
    cout<<"\n--- STOCK BAJO (<15) ---"<<endl;
    for(int i=0;i<listaProductos.size();i++){
        if(listaProductos[i].stock<15)
            cout<<"- "<<listaProductos[i].nombre<<" (Stock: "<<listaProductos[i].stock<<")"<<endl;
    }
}

void buscarComentariosPorFecha(){
    string fechaB;
    cout<<"\nFecha limite (AAAA-MM-DD): "; cin>>fechaB;
    bool encontro=false;
    for(int i=0;i<listaComentarios.size();i++){
        if(listaComentarios[i].fecha>=fechaB){
            cout<<"["<<listaComentarios[i].fecha<<"] "<<listaComentarios[i].usuario
                <<" dijo: "<<listaComentarios[i].texto<<endl;
            encontro=true;
        }
    }
    if(!encontro) cout<<"No hay comentarios."<<endl;
}

void guardarCarritoEnArchivo(){
    ofstream archivo("Carrito.txt");
    if(archivo.is_open()){
        archivo<<"idCarrito,idUsuario,nombreProducto,precioProducto"<<endl;
        for(int i=0;i<listaCarrito.size();i++){
            archivo<<listaCarrito[i].idCarrito<<","
                   <<listaCarrito[i].idUsuarioDuenio<<","
                   <<listaCarrito[i].nombreProducto<<","
                   <<listaCarrito[i].precioProducto<<endl;
        }
        archivo.close();
    }
}

void comprar(int pos){
    int idB, idC;
    cout<<"\nID del producto: "; cin>>idB;
    cout<<"ID del carrito (1 o 2): "; cin>>idC;

    if(idC<1 || idC>2){
        cout<<"Error: Un usuario solo puede tener maximo 2 carritos."<<endl;
        return;
    }

    for(int i=0;i<listaProductos.size();i++){
        if(listaProductos[i].idProducto==idB && listaProductos[i].stock>0){
            listaProductos[i].stock--;
            carritocompras item;
            item.idCarrito=idC;
            item.idUsuarioDuenio=listaUsuarios[pos].idUsuario;
            item.nombreProducto=listaProductos[i].nombre;
            item.precioProducto=listaProductos[i].precio;
            listaCarrito.push_back(item);
            cout<<"¡Agregado al carrito "<<idC<<"!"<<endl;
            guardarCarritoEnArchivo();
            return;
        }
    }
    cout<<"No disponible."<<endl;
}

void verMisCarritos(int idU){
    cout<<"\n--- MIS CARRITOS ---"<<endl;
    bool tieneAlgunProducto=false;

    for(int c=1;c<=2;c++){
        double total=0;
        bool carritoActivo=false;

        for(int i=0;i<listaCarrito.size();i++){
            if(listaCarrito[i].idUsuarioDuenio==idU && listaCarrito[i].idCarrito==c){
                if(!carritoActivo){
                    cout<<"\n Carrito "<<c<<":"<<endl;
                    carritoActivo=true;
                    tieneAlgunProducto=true;
                }
                cout<<"  - "<<listaCarrito[i].nombreProducto
                    <<" ($"<<listaCarrito[i].precioProducto<<")"<<endl;
                total+=listaCarrito[i].precioProducto;
            }
        }
        if(carritoActivo){
            cout<<"  Total Carrito "<<c<<": $"<<total<<endl;
        }
    }

    if(!tieneAlgunProducto){
        cout<<"No tienes productos en ningun carrito."<<endl;
    }
}

int IdDeCompra(){
    ordenDeCompra num;
    ifstream archivo("IdDeCompra.txt");
    if(archivo.is_open()){
        archivo>>num.iDorden;
        num.iDorden++;
        archivo.close();
    } else {
        cout<<"Error al abrir IdDeCompra.txt, se asumira ID 1"<<endl;
        num.iDorden=1;
    }
    ofstream archivo2("IdDeCompra.txt");
    if(archivo2.is_open()){
        archivo2<<num.iDorden;
        archivo2.close();
    }
    return num.iDorden;
}

void realizarFactura(int pos){
    ofstream archivo("Factura.txt");
    if(archivo.is_open()){
        ordenDeCompra sub;
        sub.iDorden=IdDeCompra();
        int idU=listaUsuarios[pos].idUsuario;

        archivo<<"========================================"<<endl;
        archivo<<"           ORDEN DE COMPRA              "<<endl;
        archivo<<"========================================"<<endl;
        archivo<<"ID de orden: "<<sub.iDorden<<endl;
        archivo<<"Usuario: "<<listaUsuarios[pos].nombre<<" ("<<listaUsuarios[pos].correo<<")"<<endl;
        archivo<<"Direccion de envio: "<<listaUsuarios[pos].direccion<<endl;
        archivo<<"Metodo de pago: "<<listaUsuarios[pos].pago<<endl;
        archivo<<"----------------------------------------"<<endl;
        archivo<<"Los productos son: "<<endl;

        vector<carritocompras> carritoRestante;

        for(int i=0;i<listaCarrito.size();i++){
            if(listaCarrito[i].idUsuarioDuenio==idU){
                archivo<<"- "<<listaCarrito[i].nombreProducto
                       <<" ($"<<listaCarrito[i].precioProducto<<")"<<endl;
                sub.subtotal+=listaCarrito[i].precioProducto;

                carritocompras itemVendido=listaCarrito[i];
                itemVendido.idCarrito=sub.iDorden;
                historialVentasGlobales.push_back(itemVendido);
            } else {
                carritoRestante.push_back(listaCarrito[i]);
            }
        }

        listaCarrito=carritoRestante;
        guardarCarritoEnArchivo();

        if(listaUsuarios[pos].bono==true){
            sub.descuento=sub.subtotal*0.5;
            listaUsuarios[pos].bono=false;
        } else{
            sub.descuento=0;
        }

        double subtotaldescuento=sub.subtotal-sub.descuento;
        sub.impuestos=subtotaldescuento*0.19;
        sub.total=subtotaldescuento+sub.impuestos+sub.envio;

        archivo<<"----------------------------------------"<<endl;
        archivo<<"Subtotal: $"<<sub.subtotal<<endl;
        if(sub.descuento>0){
            archivo<<"BONO 50% APLICADO: $"<<sub.descuento<<endl;
        }
        archivo<<"Impuestos (19% IVA): $"<<sub.impuestos<<endl;
        archivo<<"Envio fijo: $"<<sub.envio<<endl;
        archivo<<"Su total es: $"<<sub.total<<endl;
        archivo<<"========================================"<<endl;

        ordenes.push_back(sub);

        double valLim=100000;
        if(sub.total>valLim){
            listaUsuarios[pos].bono=true;
            archivo<<"\n FELICIDADES GANASTE UN BONO DEL 50% PARA TU PROXIMA COMPRA \n";
            cout<<"Felicidades Por superar los $"<<valLim<<" ganaste un bono del 50% para tu proxima compra.\n";
        }

        archivo.close();
        cout<<"\n¡Su factura se ha generado exitosamente en 'Factura.txt'!"<<endl;
    } else {
        cout<<"Error al abrir el archivo de Factura.txt"<<endl;
    }
}

void mostrarProductosMasVendidos(){
    map<int,int> conteoVentas;
    map<int,vector<int>> historialOrdenPorProducto;

    for(size_t i=0;i<historialVentasGlobales.size();i++){
        int idReal=-1;
        for(size_t j=0;j<listaProductos.size();j++){
            if(listaProductos[j].nombre==historialVentasGlobales[i].nombreProducto){
                idReal=listaProductos[j].idProducto;
                break;
            }
        }
        if(idReal!=-1){
            conteoVentas[idReal]+=1;
            historialOrdenPorProducto[idReal].push_back(historialVentasGlobales[i].idCarrito);
        }
    }

    cout<<"\n=======================================================\n";
    cout<<"      PRODUCTOS MAS VENDIDOS EN LA PLATAFORMA          \n";
    cout<<"=======================================================\n";

    if(conteoVentas.empty()){
        cout<<"No se han realizado ventas en el sistema aun.\n";
        return;
    }

    for(auto const& par:conteoVentas){
        int idProd=par.first;
        int cantidad=par.second;
        string nombreProd="";

        for(size_t i=0;i<listaProductos.size();i++){
            if(listaProductos[i].idProducto==idProd){
                nombreProd=listaProductos[i].nombre;
                break;
            }
        }

        cout<<"Producto ID: "<<idProd<<"\n";
        cout<<"  Nombre: "<<nombreProd<<"\n";
        cout<<"  -> Cantidad Total Vendida: "<<cantidad<<"\n";
        cout<<"  -> Ordenes asociadas: [ ";

        for(size_t k=0;k<historialOrdenPorProducto[idProd].size();k++){
            bool duplicado=false;
            for(size_t m=0;m<k;m++){
                if(historialOrdenPorProducto[idProd][m]==historialOrdenPorProducto[idProd][k]){
                    duplicado=true;
                    break;
                }
            }
            if(!duplicado){
                cout<<"#"<<historialOrdenPorProducto[idProd][k]<<" ";
            }
        }
        cout<<"]\n";
        cout<<"-------------------------------------------------------\n";
    }
}

void Reporte(){
    std::cout<<"\n============================\n";
    std::cout<<"  REPORTE DE LAS  GANANCIAS ";
    std::cout<<"\n============================\n";

    if(ordenes.empty()){
        std::cout<<"Aun no se han realizado ventas en la plataforma\n";
        return;
    }

    double sumaSubotales=0, sumaImpuestos=0, sumaTotales=0;

    for(int i=0;i<ordenes.size();i++){
        sumaSubotales+=ordenes[i].subtotal;
        sumaImpuestos+=ordenes[i].impuestos;
        sumaTotales+=ordenes[i].total;
    }

    std::cout<<"Total de ordenes: "<<ordenes.size()<<'\n';
    std::cout<<"Subtotal total historico: $"<<sumaSubotales<<'\n';
    std::cout<<"Impuesto total (IVA): $"<<sumaImpuestos<<'\n';
    std::cout<<"Total neto: $"<<sumaTotales<<'\n';
    std::cout<<"======================================";
}

//PUNTO A TOP 5 MENOR STOCK
void minstock(){

vector <producto> copy=listaProductos;

for(int a=0;a<copy.size();a++){
    for(int b=0;b<(int)copy.size()-1;b++){
        if(copy[b].stock>copy[b+1].stock){
            producto temp=copy[b];
            copy[b]=copy[b+1];
            copy[b+1]=temp;
        }
    }
}

std::cout<<"\n=====TOP PRODUCTOS CON MENOR STOCK=====\n";
std::cout<<"ID    | NOMBRE          | STOCK\n";
std::cout<<"------|-----------------|------\n";

int lim = ((int)copy.size()<5)?(int)copy.size():5;
for(int i=0; i<lim;i++){
    std::cout<<copy[i].idProducto<<"   | ";
    std::cout<<copy[i].nombre<<"   | ";
    std::cout<<copy[i].stock<<'\n';
}
}

//PUNTO B CANTIDAD DE COMENTARIOS
void comperdate(){
    string Busfecha;
    std::cout<<"\n ingrese la fecha (AAAA-MM-DD): ";
    std::cin>>Busfecha;

    int cant=0;
    for(int i=0;i<(int)listaComentarios.size();i++){
        if(listaComentarios[i].fecha == Busfecha){
            cant++;
        }
    }

    std::cout<<"\n======COMENTARIOS POR FECHA=====\n";
    std::cout<<"Fecha: "<<Busfecha<<"\n";
    std::cout<<"Cantidad: "<<cant<<"\n";
}

//PUNTO C PRECIO MAXY MIN
void preciomaxmin(){
    if(listaProductos.empty()){
        std::cout<<"No hay productos cargados"<<'\n';
        return;
    }

    double preciomax=listaProductos[0].precio;
    double preciomin=listaProductos[0].precio;
    string nombremax=listaProductos[0].nombre;
    string nombremin=listaProductos[0].nombre;

    for(int i=1;i<(int)listaProductos.size();i++){
        if(listaProductos[i].precio>preciomax){
            preciomax=listaProductos[i].precio;
            nombremax=listaProductos[i].nombre;
        }
        if(listaProductos[i].precio<preciomin){
            preciomin=listaProductos[i].precio;
            nombremin=listaProductos[i].nombre;
        }
    }

    std::cout<<"\n=====PRECIO MAXIMO Y MINIMO=====\n";
    std::cout<<"Maximo: $"<<preciomax<<" --> "<<nombremax<<'\n';
    std::cout<<"Minimo: $"<<preciomin<<" --> "<<nombremin<<'\n';
}

int main(){
    cargarUsuarios();
    cargarProductos();
    cargarComentarios();
    cargarCarrito(); // ← carga el carrito guardado al iniciar

    int pos=login();
    if(pos!=-1){
        int op=-1;
        while(op!=0){
            cout<<"\n1.Stock Bajo\n2.Comentarios\n3.Usuarios\n4.Comprar\n5.Mis Carritos\n6.Realizar Facturacion\n7.Ver Productos Mas Vendidos\n8.Realizar Reporte\n9.Informes\n0.Salir\nOpc: ";
            cin>>op;
            if(op==1) mostrarStockBajo();
            if(op==2) buscarComentariosPorFecha();
            if(op==3) mostrarUsuarios();
            if(op==4) comprar(pos);
            if(op==5) verMisCarritos(listaUsuarios[pos].idUsuario);
            if(op==6) realizarFactura(pos);
            if(op==7) mostrarProductosMasVendidos();
            if(op==8) Reporte();
            if(op==9){
                int subop=0;
                std::cout<<"\n=====MENU INFORMES ======\n";
                std::cout<<"1.Top 5 productos menor stock\n";
                std::cout<<"2.Comentarios por fecha exacta\n";
                std::cout<<"3.Precio maximo y minimo\n";
                std::cout<<"Opcion: ";
                std::cin>>subop;

                if(subop==1) minstock();
                if(subop==2) comperdate();
                if(subop==3) preciomaxmin();
            }
        }
    } else {
        cout<<"Error de login."<<endl;
    }
    return 0;
}