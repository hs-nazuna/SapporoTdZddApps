# SapporoTdZddApps
[TdZdd](https://github.com/kunisura/TdZdd) と [SAPPOROBDD](http://www.lab2.kuis.kyoto-u.ac.jp/minato/SAPPOROBDD/) の橋渡しライブラリ

実装予定の機能は下記の通り
* 各種制約下での DD 構築
  * グラフ制約（[FrontierBasedSearchWithVetexIndices](https://github.com/hs-nazuna/FrontierBasedSearchWithVertexIndices) の焼き直し）
    * s-t パス、サイクル、連結（木、森を含む）、シュタイナー制約
    * 辺の順序付けを行う Beam Search（優先度：低）
  * アイテムセット制約
    * 線形不等式（整数のみ）
  * DdStructure <--> ZBDD の変換
* SAPPOROBDD の拡張としての新規演算
  * DisjointJoin, JointJoin など
  * 特定のアイテムを含む/含まない
  * 特定のアイテム間の xor
  * etc...
* 最適化
  * DP
  * 近々論文にしたい手法（論文をサブミットしたら公開する）
