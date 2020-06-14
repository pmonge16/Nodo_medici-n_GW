#Importar bibliotecas
import csv
import os

#Definición de variables globales
dict = {}
cant_datos = 0

#La siguiente función se encarga de leer cada línea del archivo y
#llamar la función de traducción
def lector_lineas(archivo):
    frase = ''
    salto = False
    cant_datos = 0
    for row in archivo:
        for index in row:
            if (cant_datos == 10):
                salto = True
                cant_datos = 0
            else:
                cant_datos += 1
            frase = index
            x = traductor(frase)
            generar_secuencia(x,salto)
            salto = False

def traductor(frase):
    mascara = ''    #es una variable que guarda temporalmente los datos de contenido
    try:
        print(frase," Traductor:", dict[frase])
        return dict[frase]

    except KeyError:
        print(frase)
        contenido = ''

        for c in range(0,len(frase)):
            contenido += frase[c]

            try:
                mascara += dict[contenido]
                contenido = contenido[2:]
                if(c==(len(frase)-1)):
                    print("Valor final.:", mascara)  
                    return mascara

            except KeyError:
                if( len(contenido) > 1):
                    mascara += contenido[0]
                    contenido = contenido[1:]
                if(c ==(len(frase)-1)):
                    mascara += frase[c]
                    
                else:
                    continue

        print("Valor final.:", mascara)            
        return mascara


def diccionario():
    num = 0
    for num in range(0,52):
        if num < 12:
            dict[str(23 + num)] = chr(97 + num)
        if (num > 11)&(num < 26):
            dict[str(43  + num)] = chr(96 + num)
        if num > 25:
            dict[str(43 + num)] = chr(39 + num)


def generar_secuencia(dato, salto):
    file = open("filename.txt", "a")
    if salto==True:
        file.write(dato + "\n")
    else:
        file.write(dato + ",")
    file.close()

if __name__ == '__main__':
    archivo = 'file.txt'
    with open(archivo) as csv_file:
        csv_lector = csv.reader(csv_file, delimiter=',')
        diccionario()
        lector_lineas(csv_lector)


