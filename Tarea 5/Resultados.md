# Presentación de resultados en transferencia TCP y UDP

Los resultados que se presentan a continuación se recopilaron de promediar tres corridas para cada uno de los códigos TCP/UDP. Siendo tres tablas con:

1. Envío de **10,000** paquetes de **1440 bytes** con un tamaño de buffer de **256 bytes** (*default*).
2. Envío de **10,000** paquetes de **1440 bytes** con un tamaño de buffer de **32*1024 bytes**.
3. Envío de **10,000** paquetes de **1440 bytes** con un tamaño de buffer de **5000*1024 bytes**.

Para la comprobación de la red local, se utilizaron dos máquinas con diferentes sistemas operativos: OS X (Mac) y Ubuntu 14.04 (Linux), y se ilusta una transferencia en las siguientes imágenes:

<img src="https://raw.githubusercontent.com/RodolfoFerro/ComputerNetworks17/master/Tarea%205/server.png" width="50%"><img src="https://raw.githubusercontent.com/RodolfoFerro/ComputerNetworks17/master/Tarea%205/client.png" width="50%">

Dichos resultados se encuentran distribuidos en las siguientes tablas:

|                 |              |           TCP           |        TCP       |           UDP           |        UDP       |            UDP             |     UDP    |
| --------------- | ------------ | ----------------------- | ---------------- | ----------------------- | ---------------- | -------------------------- | ---------- |
| **Tam. buffer** | **Servidor** | **Tiempo de ejecución** | **Megabits/seg** | **Tiempo de ejecución** | **Megabits/seg** | **Paquetes perdidos (PP)** | **PP (%)** |
| *default* (256) | *localhost*  | 0.016939 seg            | 830184.780684    | 0.036986 seg            | 380211.431352    | 0                          | 0.00 %     |
| *default* (256) | Red local    | 0.029357 seg            | 479016.929523    | 0.019146 seg            | 265664.172673    | 6383                       | 68.83 %    |


|                 |              |           TCP           |        TCP       |           UDP           |        UDP       |            UDP             |     UDP    |
| --------------- | ------------ | ----------------------- | ---------------- | ----------------------- | ---------------- | -------------------------- | ---------- |
| **Tam. buffer** | **Servidor** | **Tiempo de ejecución** | **Megabits/seg** | **Tiempo de ejecución** | **Megabits/seg** | **Paquetes perdidos (PP)** | **PP (%)** |
| 32*1024 bytes   | *localhost*  | 0.069634 seg            | 202009.986103    | 0.030384 seg            | 409554.576422    | 1151                       | 11.51 %    |
| 32*1024 bytes   | Red local    | 0.323525 seg            | 43466.501816     | 0.012363 seg            | 375705.229313    | 6697                       | 66.97 %    |


|                 |              |           TCP           |        TCP       |           UDP           |        UDP       |            UDP             |     UDP    |
| --------------- | ------------ | ----------------------- | ---------------- | ----------------------- | ---------------- | -------------------------- | ---------- |
| **Tam. buffer** | **Servidor** | **Tiempo de ejecución** | **Megabits/seg** | **Tiempo de ejecución** | **Megabits/seg** | **Paquetes perdidos (PP)** | **PP (%)** |
| 5000*1024 bytes | *localhost*  | 0.151377 seg            | 93803.343695     | 0.037129 seg            | 378747.071023    | 1061                       | 10.61 %    |
| 5000*1024 bytes | Red local    | 0.332280 seg            | 42321.235103     | 0.011317 seg            | 359359.812671    | 7108                       | 71.08 %    |


### Preguntas

#### 1. Explique el por qué de sus resultados.
Se puede apreciar una mayor velocidad en transferencias UDP en comparación con transferencias TCP. Sin embargo, debe señalarse también la pérdida de paquetes en UDP.

#### 2. ¿Se perdieron paquetes en UDP? ¿Por qué?
Sí, debido a que el método UDP no se basa en conexiones; es decir, a diferencia de TCP éste método no requiere las funciones de `listen()` y `accept()` por parte del servidor, ni `connect()` por parte del cliente. Esto hace que la transferencia de paquetes sea más rápida (y más liviana), aunque crea una pérdida de confiabilidad, siendo que no se asegura una coexión y paquetes enviados son perdidos.

#### 3. ¿Cuál es mejor?
Depende de lo que se busque como resultado, pues si lo que se requiere es velocidad, UDP resulta ser mejor opción, a costa de perder información. En este sentido, las transferencias TCP tienen razón de ser más tardadas debido a la no pérdidad de paquetes.


### Referencias:
* [TCP vs. UDP](https://es.diffen.com/tecnologia/TCP-vs-UDP)
