# Trabalho Final - ED1

Trabalho Final da disciplina de Estrutura de Dados I (ED1).

## Sobre o trabalho

Uma imagem em tons de cinza pode ser vista como uma matriz de números inteiros de 1 byte.

Faça um programa cujo executável chamará imm (imm.exe no windows; imm no linux).

###### O programa vai operar em linha de comando e terá as seguintes funcionalidades (use argc e argv para lidar com os parâmetros):

* imm -open file.txt
  * Abre uma imagem (formato texto) e mostra os valores dos pixels na tela
* imm -convert file.txt file.imm
  * Converte uma imagem no formato file.txt para o formato file.imm
* imm -open file.imm
  * Abra uma imagem (formato binária) e mostra os valores dos pixels na tela
* imm -segment thr file.imm segfile.imm
  * Faz o thresholding (limiarização da imagem) com um valor thr da imagem file.imm e escreve o resultado em segfile.imm
* imm -cc segfile.im outfile
  * Detecta os componentes conexos de uma imagem
* imm -lab imlab.txt imlabout.txt
  * Mostra o caminho a ser percorrido em um labirinto
* imm -outro-comando-que-não-existe
  * Mostra uma mensagem de erro de comando não encontrado

###### Etapas sugerida para construção do programa

* Etapa 1: um programa que é capaz de entender todos os comandos mas não vai executar nada ainda. Somente chamará funções vazias que mostrarão os argumentos passados pela linha de comando
* Etapa 2: comandos open (texto); convert; open (binário)
* Etapa 3: segment e componentes conexos
* Etapa 4: imagem labirinto

###### Condições

* Criar um TAD para lidar com as funções e operações da imagem baseando-se no TAD de matriz (pode-se alterar o TAD matriz ou usá-lo)
* Use os TADs criados ao longo do curso quando necessário

## Para compilar com gcc no terminal do Linux

Com o terminal, vá a pasta em que está o código fonte, e rode o seguinte comando:

```
gcc -c *.c
```

E depois gerar o executável com todos '.o' gerados:

```
gcc -o imm *.o
```

## Opcional: Organizar '.o' e executável

Na pasta da compilação.

Crie os diretórios 'obj/', para o '.o', e bin/, para o executável:

```
mkdir obj/
mkdir bin/
```

Depois mova o arquivo objeto e o executável para suas pastas:

```
mv *.o obj/
mv imm bin/
```

## Para usar o programa

O que cada função faz está descrito na parte 'Sobre o trabalho'.

Estão disponibilizados alguns arquivos para testes no diretório images, deste repositório.

Função open:

```
imm -open file.txt
```

Função convert:

```
imm -convert file.txt file.imm
```

Função open com arquivo '.imm':

```
imm -open file.imm
```

Função segment:

```
imm -segment thr file.imm segfile.imm
```

Função cc:

```
imm -cc segfile.im outfile
```

Função lab:

```
imm -lab imlab.txt imlabout.txt
```
