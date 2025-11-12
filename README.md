Atualização OTA - ESP32
Todo o código relacionado à implementação e análise de OTA usando ESP32 via ESP-IDF

Tópicos abordados:

- Implementação de OTA via HTTP/HTTPS  
- Alterações na tabela de partições  
- HTTP/HTTPS  
- Geração de certificados via ESP-IDF  
- Assinatura de código  
- Métricas de tempo de CPU via estatísticas de tempo de execução do FreeRTOS  

1. Instalar o Visual Studio Code

- Baixe e instale o Microsoft VSCode
- Instale a extensão **Espressif IDF** na aba de extensões

2. Instalar o ESP-IDF

- Abra a paleta de comandos (Ctrl+Shift+P)
- Execute: `ESP-IDF: Configure ESP-IDF extension`
- Siga o assistente para instalar o ESP-IDF e os pacotes necessários

3. Criar um projeto baseado no exemplo Blink

- Execute: `ESP-IDF: Create project using template`
- Escolha o exemplo `blink`
- Defina o nome e o diretório do projeto
- Abra o projeto no VS Code

4. Acessar o menu de configurações (menuconfig)

- Execute: `ESP-IDF: Open ESP-IDF Menuconfig`
- Navegue pelas opções para configurar o projeto

5. Alterar a tabela de partições

- Crie ou edite o arquivo `partitions.csv` (Para a implementacao foi utilizado o modelo Factory app, two OTA definitions)

6. Ativar suporte a OTA via HTTPS

- No menuconfig, vá em `Component config > ESP HTTPS OTA`
- Ative a opção de suporte a OTA via HTTPS
- Inclua o código de atualização OTA no firmware usando `esp_https_ota`

7. Gerar estatísticas de tempo de execução

- No menuconfig, vá em `Component config > FreeRTOS`
- Ative a opção `Enable FreeRTOS runtime stats`
- Configure o temporizador para coleta de estatísticas, se necessário

8. Verificar configuração de assinatura de código

- No menuconfig, vá em `Security features > Secure boot and flash encryption`
- Ative o suporte a `Code signing`
- Configure a chave de assinatura e opções de verificação

9. Compilar e gravar o firmware

- Execute: `ESP-IDF: Build project`
- Conecte o dispositivo e execute: `ESP-IDF: Flash device`

10. Monitorar a saída serial

- Execute: `ESP-IDF: Monitor device`
- Verifique os logs para confirmar OTA, estatísticas e assinatura de código


