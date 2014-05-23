# ismatch

与えられた希望表を元に、２グループ間の [安定マッチング][smp] を求める。  
アルゴリズムは [Gale-Shapley][smp] のアルゴリズムです。  
内部で [icsv][icsv], [ilist][ilist], [iutil][iutil] を使ってます。  

## 注意！

このプログラムはお勉強のために実装したものであり、実用レベルではありません！

## 入力

入力は２つの希望表である。  
各希望表は n 行 n 列の表であり、各要素は 0 ~ n-1 の整数である。  
希望表の i 行目はそのグループの i 番目のユーザの希望リストである。  
i 行 j 列の数字はユーザ i さんが j 番目に希望している相手である。  
つまり希望リストは {0,..,n-1} の任意の順列である。  
以下に例を示す。これは [wiki][smp] の希望表である。

    # men.txt
    0 1 2 3
    2 1 0 3
    0 1 3 2
    2 0 1 3

    # women.txt
    0 1 2 3
    1 0 3 2
    1 2 0 3
    0 3 2 1

## 出力

例えば上記のファイルを入力とすると以下の出力を得る。

    0 0
    1 3
    2 3
    3 1

例えば 3 1 は men.txt の 3 と women.txt の 1 がマッチングしたとを意味する。

## 余談

実装では、希望リストは任意の整数列としている。
{0,...,n-1} 以外の数が与えれたらそれを無視し、
長さ n 以上が与えられた場合はそれ以降を無視する。
マッチングは必ず希望リストに含まれる相手とのみ成立しうる。

## method

    ismatch *ismatch_read(const char *_fm, const char *_fw);

希望表 fm, fw より ismatch インスタンスを生成する。

    void ismatch_free(ismatch *_ism);

ismatch インスタンス _ism を free する。

    void ismatch_match(ismatch *_ism);

ismatch インスタンス _ism のマッチングを行う。

## 使い方

main.c を参照してください！

\ リンク
[icsv]:  https://github.com/masakazu-ishihata/icsv  "icsv"
[ilist]: https://github.com/masakazu-ishihata/ilist "ilist"
[iutil]: https://github.com/masakazu-ishihata/iutil "iutil"
[smp]: http://ja.wikipedia.org/wiki/安定結婚問題 "安定結婚問題"

