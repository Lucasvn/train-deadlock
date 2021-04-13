# Train Deadlock

#### Projeto desenvolvido para a disciplina de Sistemas Operacionais (BTI - IMD)

## OBJETIVO:

Aplicar os conhecimentos de gerenciamento de threads e regiões críticas através do uso de semáforos e mutex, com o intuito de evitar deadlocks e condições de corrida entre os trens.

## DESAFIO:

O trabalho consiste no desenvolvimento de uma malha ferroviária que comporta 5 trens, todos circulando no sentido horário.

Cada trem deve circular em sua malha sem colidir com os demais trens.

O código deve considerar a existência de 7 regiões críticas
diferentes (regiões de colisão entre trens) destacadas e enumeradas na imagem a seguir:

![](https://i.imgur.com/2T5i1r1.png)

## REQUISITOS:

Para rodar este projeto, será necessário ter instalado o QTCreator:

```
sudo apt install qtcreator build-essential qt5-default
```

Após isso, clone este projeto:

```
git clone git@github.com:Lucasvn/train-deadlock.git
```

E abra o projeto no QTCreator.

Com o projeto aberto, basta pressionar `ctrl+r` para buildar e executar o projeto.
