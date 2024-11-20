# GraffiTech  

GraffiTech es un proyecto que combina arte y tecnología para transformar movimientos en el aire en una experiencia de graffiti digital.  

Este proyecto utiliza hardware como el ESP32, un giroscopio MPU6050 y un encoder rotativo para crear una experiencia inmersiva y autónoma.  

## Características  

- **Spray Can Digital**: Un dispositivo que emula el funcionamiento de un aerosol de graffiti real.  
- **Sincronización en Tiempo Real**: Conexión Wi-Fi con una aplicación en React para un lienzo digital interactivo.  
- **Optimización de Recursos**: Comunicación Bluetooth con el giroscopio para evitar la saturación de medios y distribución eficiente de la carga entre los núcleos del ESP32.  
- **Control Avanzado**: El encoder rotativo permite cambiar colores y activar la función de pintado.  
- **Indicadores Visuales**: Un aro de LEDs muestra el color seleccionado en tiempo real.  
- **Portabilidad**: Alimentado por un power bank integrado, permitiendo un uso completamente autónomo.  

## Tecnologías Utilizadas  

- **Hardware**:  
  - ESP32  
  - Giroscopio MPU6050  
  - Encoder Rotativo  
  - LEDs NeoPixel  

- **Software**:  
  - Framework React para la interfaz de usuario.  
  - Conexión Wi-Fi y Bluetooth mediante las bibliotecas BLEMouse y WebSocket.  

## Instalación  

1. **Clonar el repositorio**:  
   ```bash
   git clone https://github.com/Ciroop/graffitech.git  
   cd graffitech
  
## Configuración del Firmware  

1. Asegúrate de tener instalado el entorno de desarrollo de Arduino.  
2. Instala las bibliotecas necesarias para trabajar con el ESP32.  
3. Carga el archivo `.ino` al ESP32 utilizando el IDE de Arduino.  

## Ejecución de la Aplicación  

1. Dirígete al directorio `/app`.  
2. Instala las dependencias de React:  
   ```bash
   npm install
   npm start

## Conexión del Dispositivo  

1. Sincroniza el ESP32 con la aplicación mediante Wi-Fi.  
2. Asegúrate de que el giroscopio esté correctamente emparejado vía Bluetooth.  

## Contacto  

Para preguntas o sugerencias, puedes contactarnos en:  

- **Email**:
 - [lucaspanzera13@gmail.com](mailto:lucaspanzera13@gmail.com) 
 - [cerutticiro1@gmail.com](mailto:cerutticiro1@gmail.com)
  
- **LinkedIn**:
 - [Lucas Panzera](https://www.linkedin.com/in/panzeralucas/)  
 - [Ciro Cerutti](https://www.linkedin.com/in/ciro-cerutti-7797012b1/)  


