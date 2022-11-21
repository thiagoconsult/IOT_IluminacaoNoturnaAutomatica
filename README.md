# IOT_IluminacaoNoturnaAutomatica
Objetos Inteligentes Conectados - Projeto Iluminação Noturna Automática.

Este projeto apresenta um esquema de comunicação entre sensores e atuadores via protocolo MQTT. Quem quiser reproduzir, precisará montar dois esquemas:

Esquema transmissor: composto por um kit NodeMCU com WiFi integrador (no projeto foi usado o modelo ESP-12F), um sensor LDR de luminosidade, jumpers macho/macho, protoboard de 400 ou 830 furos, um power bank para alimentação do esquema e um cabo usb/micro-usb para conexão do kit NodeMcu com o computador para escrita e compilação do software embarcado.

Esquema receptor: composto por um módulo relé com entrada para encaixe do microcontrolador ESP01, um microcontrolador ESP01 com WiFi integrado, uma protoboard de 400 ou 830 furos, um controlador de tensão, uma fonte de 9V/1A, um adaptador USB para conexão do ESP01 com o computador, para gravação do sistema embarcado e um abajur/luminária.

Para o funcionamento do projeto, é necessário que o esquema transmissor tenha um software embarcado capaz de ler a luminosidade através do sensor LDR. Através da análise desta leitura, deve ser enviado um pacote para o Broker com instruções para o esquema receptor realizar a ação de ligar ou desligar o abajur.

Com os componentes esquemas devidamente montados e com os softwares embarcados nos microcontroladores, é possível verificar a comunicação, quase que instantânea entre os esquemas transmissor e receptor.
