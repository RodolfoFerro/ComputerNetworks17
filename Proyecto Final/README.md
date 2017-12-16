# Proyecto final: Internet of Things (IoT) 📡📲

La idea general del proyecto es el presentar en general lo que es IoT, pasando por un contexto histórico, aplicaciones y casos reales de uso; concluyendo con el desarrollo de un mini-proyecto utilizando del [módulo Wi-Fi ESP8266](https://electronilab.co/tienda/nodemcu-board-de-desarrollo-con-esp8266-wifi-y-lua/) para conexión con dispositivos y control de equipo electrónico, en este caso el estado de un LED.

La presente carpeta contiene una serie de scripts desarrollados para la presentación del proyecto final del curso. Dichos scripts son básicamente dos tipos de archivos: scripts de Arduino (`.ino`) y scripts de Python (`.py`).

## Programación del módulo WiFi

Para la programación del circuito, se utilizó directamente el [IDE de Arduino](https://www.arduino.cc/en/Main/Software). Además, es necesaria la instalación de las librerías del módulo, lo cualpuede hacerse [siguiendo esta guía](https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/installing-the-esp8266-arduino-addon).

### Diagrama del circuito

Un diagrama del circuito se muestra aproximación:

<img src="https://raw.githubusercontent.com/RodolfoFerro/ComputerNetworks17/master/Proyecto%20Final/imgs/sketch_1.jpg" width="45%">

Como puede apreciarse, el módulo ESP8266 queda sobre una protoboard y el LED queda conectado al pin D7 (que es mapeado al 13 desde la parte programática) y al pin de tierra (GRD).

### Scripts de arduino

Los scripts de Arduino están contenidos cada uno en su respectiva carpeta y son los siguientes:
- [`Light_Test.ino`](https://github.com/RodolfoFerro/ComputerNetworks17/blob/master/Proyecto%20Final/Arduino/Light_Test/Light_Test.ino) es un script de prueba para encender y apagar el LED de manera intermitente y comprobar la conexión correcta del circuito y la carga del programa al módulo.
- [`WiFi_Light.ino`](https://github.com/RodolfoFerro/ComputerNetworks17/blob/master/Proyecto%20Final/Arduino/Wifi_Light/WiFi_Light.ino) es un script que carga el código principal de conexión vía WiFi para controlar el estado del LED (encendido/apagado). **En este script es importante camiar el ssid por el nombre de la red y la contraseña por la contraseña de la misma, si no, el módulo no podrá conectarse para realizar el enío de datos.**


El segundo código –*el principal*– imprime la dirección IP en la consola `COMXX` (donde `XX` es el puerto al que se conectó el módulo), que contiene el estado del LED y controles para cambiarlo, como se muestra en las siguienes imágenes, que son capturas de pantalla del navegador en la dirección ya mencionada:

<img src="https://raw.githubusercontent.com/RodolfoFerro/ComputerNetworks17/master/Proyecto%20Final/imgs/off.png" width="50%"><img src="https://raw.githubusercontent.com/RodolfoFerro/ComputerNetworks17/master/Proyecto%20Final/imgs/on.png" width="50%">

Cabe mencionarse que ambos códigos del circuito se encuentran comentados para una mejor comprensión.

## Integración inteligente (*IoT style!*)

Por su parte, los scripts de Python permiten aproximar de una manera distinta ––*e inteligente*–– el control del estado del LED, digamos de una manera más estilo IoT.


### Requerimientos

Además de [Python 3.6](https://www.python.org/downloads/) (que es la versión que estoy utilizando), una serie de pquetes son requeridos, los cuales se enlistan a continuación:

- [Requests](http://docs.python-requests.org/en/master/), para el envío de peticiones GET.
- [Flask](http://flask.pocoo.org/), para la creación del servidor web de peticiones.
- [Flask-ask](http://flask-ask.readthedocs.io/en/latest/), que es una [extensión de Flask](http://flask.pocoo.org/extensions/) para el manejo de peticiones de Amazon Alexa.
- [Python-telegram-bot](https://python-telegram-bot.org/), para la creación del bot de Telegram.

La ventaja de todos estos paquetes es que son fácilmente instalables con [`pip`](https://pip.pypa.io/en/stable/), que es un gestor de paquetes de Python.

Así, la instalación de cada paquete puede hacerse así:
```bash
pip install <paquete>
```

Peeero... He creado un archivo de requermientos ([`requirements.txt`](https://github.com/RodolfoFerro/ComputerNetworks17/blob/master/Proyecto%20Final/Python/requirements.txt)) para que puedas instalar todos los requerimientos de un sólo comando. Esto lo haces así:
```bash
pip install -r requirements.txt
```

Además de esto, se requerirá un par de cosas extra:
* Para poder tener acceso a las funcionalidades del bot de Telegram, debe primero crearse un bot. Para ello, hay que hacer la solicitud a [BotFather](https://t.me/BotFather).
* Para poder subir la Alexa Skill al portal de Amazon, es decir, hacer el deployment y poder probar la aplicación desde un dispositivo con Alexa, será necesario descargar [ngrok](https://ngrok.com/) y [crear una Alexa Skill en el portal de Amazon](https://developer.amazon.com/docs/custom-skills/deploy-a-sample-skill-to-aws-lambda.html).

### Scripts de Python

Los scripts de Python contenidos en esta parte son los siguientes:

- [`turn_light.py`](https://github.com/RodolfoFerro/ComputerNetworks17/blob/master/Proyecto%20Final/Python/turn_light.py) es un script que envía una petición GET al url que se especifica al momento de correr el programa, junto con el estado, para a través de peticiones desde terminal tener un control sobre el estado del LED.
- [`telegram_bot.py`](https://github.com/RodolfoFerro/ComputerNetworks17/blob/master/Proyecto%20Final/Python/telegram_bot.py) es el script con el bot de Telegram para controlar el estado del LED. Es necesario tener el token del API en un archivo `credentials.py`.
- [`alexa_skill.py`](https://github.com/RodolfoFerro/ComputerNetworks17/blob/master/Proyecto%20Final/Python/alexa_skill.py) es el script con la Alexa Skill para controlar el estado del LED utilizando Amazon Alexa.


#### HTTP Requests (GET):

El primer script, [`turn_light.py`](https://github.com/RodolfoFerro/ComputerNetworks17/blob/master/Proyecto%20Final/Python/turn_light.py), se corre de la siguiente manera:

```bash
python turn_light.py -u "http://192.168.x.x/" -s STATE
```
Donde `STATE` es el estado al que queremos cambiar el LED, digamos `ON`/`OFF` y el url debe ser especificado con el arrojado por la consola del puerto desde el IDE de Arduino.

Este script controla, como ya se mencionó, el estado del LED desde la terminal.


#### Telegram bot:

El segundo script, [`telegram_bot.py`](https://github.com/RodolfoFerro/ComputerNetworks17/blob/master/Proyecto%20Final/Python/telegram_bot.py), requiere tener un script `credentials.py`con el token del API que devuelve  [BotFather](https://t.me/BotFather). Con ello, la manera de correr el bot es la siguiente:
```bash
python telegram_bot.py -u "http://192.168.x.x/"
```

Donde debe ponerse la dirección IP real obtenida por el dispositivo en la consola del IDE de Arduino.


#### Alexa Skill:

El tercer script, [`alexa_skill.py`](https://github.com/RodolfoFerro/ComputerNetworks17/blob/master/Proyecto%20Final/Python/alexa_skill.py), hace mancuerna con [ngrok](https://ngrok.com/), que es una aplicación para abrir túneles seguros al *localhost*.

Para el tercer script, se requerirán dos tabs/ventanas de la terminal, una para el servidor Flask que manipula la interacción con la aplicación de Alexa y la otra para crear el túnel que se sube al [portal de desarrolladores de Amazon](https://developer.amazon.com/).

Para correr el tercer script en la primer ventana hacemos lo siguiente:
```bash
python alexa_skill.py -u "http://192.168.x.x/"
```

Donde debe ponerse la dirección IP real obtenida por el dispositivo en la consola del IDE de Arduino.

En la segunda ventana corremos ngrok abriendo el puerto 5000 del *localhost* (que es el default de Flask) como sigue:
```bash
ngrok http 5000
```

**Debe mencionarse que en caso de que se haya modificado el puerto, este se pasa como argumento en consola a la hora de correr ngrok.**

Esto abre el túnel creando un enlace seguro (http**s**) parecido a este: `https://xxxxxxxx.ngrok.io`. Dicho enlace es el que se pasará al portal de desarrolladores de Amazon.

## Probando la integración inteligente

Ya que se cuenta con todo el desarrollo hasta este punto, pueden probarse los scripts de integración inteligente.

#### Telegram bot:
Cuando el bot de Telegram se encuentre corriendo, puede iniciarse con el comando `/start`. Este arrojará un texto con dos botones para poder controlar el estado del LED.

Para reiniciar el bot (tras haber modificado el estado del LED) basta volver al usar el mismo comando, `/start`.

#### Alexa Skill:
Ya que la Alexa Skill se encuentra en línea, puede probarse directamente con los dispositivos [Echo](https://www.amazon.com/dp/B01DFKC2SO/ref=ods_xs_dp_oop) de Amazon (Dot, Plus, etc.) o utilizando el servicio de [Echosim.io](https://echosim.io/).

El comando para invocar al bot es:
> ––*Alexa, open Light Turner, please.*

Tras ello, sólo basta resonder si queremos cambiar el estado del LED.

### DEMO
Un demo del funcionamiento puede ser encontrado en el siguiente video:
