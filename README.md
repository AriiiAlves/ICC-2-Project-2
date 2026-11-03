# ICC-2-Project-2

# Objetivo

Serão dados textos. As palavras devem ser armazenadas em uma tabela hash, junto com a contagem de número de ocorrências da palavra.

Deve ter operção de inserção, remoção e busca.

## Hashing Estático Fechado

- Estático - Tabela tem tamanho fixo m. Não cresce nem faz rehash automático quando enche.
- Fechado - Não há listas encadeadas, tudo fica na tabela. Um elemento também pode acabar em uma posição diferente da que a hash indicou.

## Sondagem quadrática

A sondagem linear tenta: h(k), h(k) + 1, h(k) + 2...

Isso causa agrupamento primário: formam-se blocos contíguos de posições ocupadas, que crescem e deixam as buscas lentas. A sondagem quadrática faz:

$$ h(k,i) = (h(k) + c_1 \cdot i + c_2 \cdot i^2) mod m, i=0,1,2,... $$