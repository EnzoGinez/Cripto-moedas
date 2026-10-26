# 📈 Exchange de Criptomoedas

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Academic Project](https://img.shields.io/badge/Projeto-Acad%C3%AAmico-blue?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Conclu%C3%ADdo-success?style=for-the-badge)

Projeto final desenvolvido para a disciplina **CCP230 - Desenvolvimento de Algoritmos** do **Centro Universitário FEI**, sob a orientação da Profa. Gabriela Biondi.

O projeto consiste na criação de uma plataforma de Exchange de Criptomoedas via console, permitindo a compra, venda e administração de ativos digitais, com persistência de dados em arquivos de texto.

---

## 🛠️ Tecnologias Utilizadas

- **Linguagem:** C (Obrigatório)
- **Persistência de Dados:** Arquivos de texto (`.txt` ou similar)
- **Interface:** Console/Terminal

---

## 🪙 Criptomoedas e Regras de Negócio

A plataforma opera com a moeda base **Real (BRL)** e suporta 3 tipos de criptomoedas, cada uma com suas respectivas taxas de transação:

| Criptomoeda | Taxa de Compra | Taxa de Venda |
| :--- | :---: | :---: |
| **Bitcoin (BTC)** | 2% | 3% |
| **Ethereum (ETH)** | 1% | 2% |
| **Ripple (XRP)** | 1% | 1% |

**Regra de Cotação:** A atualização das cotações é feita através de geração de números aleatórios, garantindo uma variação máxima de **+5%** e mínima de **-5%** em relação ao valor anterior.

---

## ⚙️ Funcionalidades

O sistema é dividido em duas áreas principais, dependendo do tipo de usuário (Nota: As funções de Administrador são exclusivas para projetos desenvolvidos em dupla).

### 👤 Painel do Investidor
O acesso é feito via CPF (somente números) e senha numérica de 6 dígitos.
- **Consultar Saldo:** Exibe os saldos atualizados de Reais, Bitcoin, Ethereum e Ripple.
- **Consultar Extrato:** Histórico completo de transações detalhando data, hora, operação (+ ou -), moeda, valor da cotação (CT), taxa cobrada (TX) e saldos pós-operação.
- **Depositar Reais:** Adiciona fundos à carteira em BRL.
- **Sacar Reais:** Retira fundos da carteira (não permite saldo negativo).
- **Comprar Criptomoedas:** Consulta a cotação atual, calcula taxas e efetua a compra (não permite saldo negativo).
- **Vender Criptomoedas:** Consulta a cotação atual, calcula taxas e efetua a venda.
- **Atualizar Cotação:** Flutua o mercado de criptomoedas (±5%).

---

## 🚀 Como Executar o Projeto

**Pré-requisitos:** Um compilador C (como o `gcc`) instalado na sua máquina.

1. Clone o repositório:
```bash
git clone https://github.com/SEU_USUARIO/NOME_DO_REPOSITORIO.git
```

2. Navegue até a pasta do projeto:
```bash
cd NOME_DO_REPOSITORIO
```

3. Compile o código-fonte:
```bash
gcc main.c -o exchange
```
*(Nota: Substitua `main.c` pelo nome correto do seu arquivo principal, e adicione outros arquivos `.c` caso tenha modularizado o código).*

4. Execute o programa:
```bash
./exchange  # No Linux/Mac
exchange.exe # No Windows
```

---

## 📁 Estrutura de Arquivos

*Como regra do projeto, nenhuma informação é perdida ao fechar o programa. Todos os dados de usuários, senhas, saldos, cotações e extratos são persistidos em arquivos locais gerados automaticamente pelo sistema.*
