# mesoShell

**mesoShell** é um interpretador de comandos (shell) simples desenvolvido em C para sistemas Linux.  
O objetivo do projeto é reproduzir funcionalidades básicas de um shell tradicional, permitindo executar comandos do sistema, além de implementar comandos internos e modos de execução específicos.

Este projeto foi desenvolvido com fins educacionais para estudar conceitos como:

- Processos
- Execução de comandos no sistema operacional
- Manipulação de strings
- Gerenciamento de processos (`fork`, `exec`, `wait`)
- Desenvolvimento de interpretadores de comandos

---

# Funcionalidades

O **mesoShell** implementa as seguintes funcionalidades:

## Execução de comandos do sistema

Permite executar comandos disponíveis no sistema Linux, como:

```bash
ls
pwd
mkdir
cat arquivo.txt

### 🐧 Linux / WSL (Ubuntu)



### Como reproduzir (compilar, executar, testar e validar)

```bash
make
./mesoShell (interativo)
./mesoShell (bash)

```

