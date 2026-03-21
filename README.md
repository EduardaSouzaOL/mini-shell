# 🐚 mesoShell

![C](https://img.shields.io/badge/language-C-blue?style=flat-square&logo=c)
![Linux](https://img.shields.io/badge/platform-Linux-yellow?style=flat-square&logo=linux)
![Status](https://img.shields.io/badge/status-educational-green?style=flat-square)
![License](https://img.shields.io/badge/license-MIT-lightgrey?style=flat-square)

**mesoShell** é um interpretador de comandos (shell) simples desenvolvido em C para sistemas Linux. O objetivo do projeto é reproduzir funcionalidades básicas de um shell tradicional, permitindo executar comandos do sistema, além de implementar comandos internos e modos de execução específicos.

> Projeto desenvolvido com fins educacionais para aprofundar conhecimentos em sistemas operacionais e programação em C.

---

## 📚 Conceitos Abordados

| Conceito | Descrição |
|---|---|
| **Processos** | Criação e gerenciamento de processos com `fork` e `wait` |
| **Execução de comandos** | Substituição de imagem de processo com a família `exec` |
| **Manipulação de strings** | Parsing e tokenização da linha de comando |
| **Interpretadores** | Leitura, análise e despacho de comandos (REPL) |

---


### Comandos Internos (Built-ins)
O mesoShell implementa os seguintes comandos nativos, executados diretamente pelo shell sem criação de processo filho:

| Comando | Descrição |
|---|---|
| `cd <dir>` | Muda o diretório de trabalho atual |
| `exit` / `quit` | Encerra o shell |
| `echo <texto>` | Exibe texto na saída padrão |

### Funcionalidades Avançadas

#### 🔀 Redirecionamento de Entrada/Saída
Redireciona a entrada e saída dos comandos para arquivos:

```bash
# Redirecionar saída para arquivo (sobrescreve)
ls -l > lista.txt

# Redirecionar saída para arquivo (acrescenta)
echo "nova linha" >> lista.txt

# Redirecionar entrada de arquivo
sort < nomes.txt
```

#### ⚙️ Execução em Background
Permite executar processos de forma assíncrona sem bloquear o shell:

```bash
sleep 10 &
./processo_longo &
```

#### 🌐 Variáveis de Ambiente
Suporte à leitura e uso de variáveis de ambiente do sistema:

```bash
echo $HOME
echo $PATH
echo $USER
```

---

## 🛠️ Tecnologias

- **Linguagem:** C (padrão C99)
- **Plataforma:** Linux (também compatível com WSL)
- **Build:** `make` / `gcc`
- **System Calls:** `fork`, `execvp`, `wait`, `waitpid`, `chdir`, `getcwd`, `dup2`, `open`

---

## 📁 Estrutura do Projeto

```
mesoShell/
├── src/
│   ├── main.c          # Ponto de entrada e loop principal (REPL)
│   ├── parser.c        # Parsing e tokenização dos comandos
│   ├── builtins.c      # Implementação dos comandos internos
│   └── executor.c      # Execução de comandos externos via fork/exec
├── include/
│   └── mesoshell.h     # Cabeçalhos e protótipos de funções
├── Makefile
└── README.md
```

---

## 🚀 Como Usar

### Pré-requisitos

- Sistema Linux ou WSL (Ubuntu recomendado)
- `gcc` instalado
- `make` instalado

```bash
# Verificar gcc
gcc --version

# Verificar make
make --version
```

### Instalação e Compilação

```bash
# Clone o repositório
git clone https://github.com/seu-usuario/mesoShell.git
cd mesoShell

# Compile o projeto
make
```

### Execução

**Modo Interativo:**
```bash
./mesoShell
```

O shell iniciará e exibirá um prompt aguardando seus comandos:
```
mesoShell> ls
mesoShell> cd Documents
mesoShell> echo "olá!"
mesoShell> exit
```

**Modo Batch (Script):**
```bash
./mesoShell script.sh
```

### Limpeza

```bash
make clean
```

---

## 🧪 Exemplos de Uso

```bash
# Navegar entre diretórios
mesoShell> cd /home/usuario
mesoShell> pwd

# Redirecionar saída para arquivo
mesoShell> ls -la > arquivos.txt
mesoShell> echo "log de execução" >> log.txt

# Usar variável de ambiente
mesoShell> echo $HOME
mesoShell> echo $USER

# Executar processo em background
mesoShell> sleep 5 &

# Encerrar o shell
mesoShell> exit
```

---

## ⚙️ Funcionamento Interno

O mesoShell segue o ciclo clássico de um shell (REPL):

```
┌─────────────┐
│  Ler linha  │  ← leitura da entrada do usuário
└──────┬──────┘
       │
┌──────▼──────┐
│  Tokenizar  │  ← separar comando, argumentos e operadores
└──────┬──────┘
       │
┌──────▼──────┐
│  Builtin?   │  ← cd / echo / exit → executa direto
└──────┬──────┘
       │ não
┌──────▼──────┐
│Redirecionam.│  ← configura dup2 se houver >, <, >>
└──────┬──────┘
       │
┌──────▼──────┐
│ fork + exec │  ← cria processo filho e executa o comando
└──────┬──────┘
       │
┌──────▼──────┐
│ background? │  ← se & presente, não bloqueia com wait
└──────┬──────┘
       │ não
┌──────▼──────┐
│    wait     │  ← aguarda conclusão do processo filho
└─────────────┘
```

---

## 📌 Limitações Conhecidas

- Não suporta pipes (`|`) para encadeamento de comandos
- Sem expansão de curingas (`*`, `?`) — glob
- Histórico de comandos não implementado
- Sem suporte a scripts com estruturas de controle (`if`, `for`, `while`)

---

## 🎓 Contexto Acadêmico

Este projeto foi desenvolvido como parte de estudos em **Sistemas Operacionais** com foco em:
- Compreender como shells tradicionais (bash, sh) funcionam internamente
- Praticar programação de sistemas em C
- Explorar chamadas de sistema POSIX

---

## 👤 Autor

**Seu Nome**
- GitHub: [@seu-usuario](https://github.com/EduardaSouzaOL)
- Email: cc.mariaeduardasouza@email.com

---

## 📄 Licença

Este projeto está sob a licença MIT. Consulte o arquivo [LICENSE](LICENSE) para mais detalhes.
