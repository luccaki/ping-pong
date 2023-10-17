
# Ping Pong

Desenvolvidor por Lucca Ianaguivara Kisanucki, aluno da UFABC, com RA 11201812090.

[Clique para jogar](https://luccaki.github.io/ping-pong/)

A aplicação consiste básicamente do jogo ping pong. 
Possui 3 objetos, o slider da esquerda, o slider da direita e uma bola.
Podemos controlar os sliders usando as teclas 'W', 'S', 'Up' e 'Down'.

Temos algumas opções no menu, além de mostrar o placar do jogo.

Um botão para iniciar e reiniciar o jogo.

Um botão de pause e continuar.

Um botão para mudar a cor dos objetos, gerando uma nova cor aleatoriamente.

Um botão que ativa o 'Fire Mode', aumenta a velocidade da bola a cada 5 segundos.

Um barra para controlar a velocidade da bola.

Uma lista dropdown para selecionar o número de pontos necessários para a vitoria.

## Implementação:
A bola foi implementado como um ponto (x, y), e em volta desse ponto desenhamos um quadrado, para o movimento da bola foi criado um vetor também (x, y) que possui valores de -1 a +1, representando a velocidade que o ponto se mexe em cada eixo.

Para os sliders foi definido uma altura de 0.3, e duas variaveis que define a posição delas no eixo y, o valor dessa variavel é contrlado pelas teclas, podendo aumentar e diminuir dentro do limite de -1 a +1.

Ao iniciar o jogo o vetor de velocidade é difinido aleatoriamente, jogando a bolinha para algum dos lados.

Foi implementado também a colisão da bolinha com os sliders, e também com a borda da tela. Sempre que há uma colisão o vetor velocidade tem seu sentido alterado.

Para pontuação, quando o eixo x da bolinha passa de +1 ou menor que -1, é contabilizado um ponto para um jogador. Caso o número de pontos seja maior ou igual ao valor da variavel que armazena os pontos necessários para a vitoria, o jogo se reinicia.

Para pausar o jogo apenas coloco o vetor velocidade como (0,0), e para continuar retorno ao valor original do vetor.

Todos os objetos usa o mesmo VAO e VBO, apenas faço o desenho de cada um por vez. E todos os objetos compartilham o memso VBOColors tambem, possuindo os mesmos shaders.
