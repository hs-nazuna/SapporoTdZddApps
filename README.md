# SapporoTdZddApps
[TdZdd](https://github.com/kunisura/TdZdd) と [SAPPOROBDD](http://www.lab2.kuis.kyoto-u.ac.jp/minato/SAPPOROBDD/) の橋渡しライブラリ

実装予定の機能は下記の通り
* 各種制約下での DD 構築
  * グラフ制約（[FrontierBasedSearchWithVetexIndices](https://github.com/hs-nazuna/FrontierBasedSearchWithVertexIndices) の焼き直し）
    * [ ] s-t パス
    * [ ] サイクル
    * [ ] 連結（木、森を含む）
    * [ ] シュタイナー制約
  * アイテムセット制約
    * [x] 線形不等式（整数のみ）
  * 関連する機能
    * [x] DdStructure <--> ZBDD の変換
    * [ ] 辺の順序付けを行う Beam Search（優先度：低）
* SAPPOROBDD の拡張としての新規演算
  * [x] Power set
  * [ ] DisjointJoin
  * [ ] JointJoin
  * [ ] 特定のアイテム集合に着目したZDDへの変換（グラフ制約のZDDから頂点変数だけ抜き出すときなどに便利）
  * [ ] 特定のアイテムを含む/含まない（OnSet などの別名）
  * [ ] 特定のアイテム間の xor
* 最適化
  * [ ] DP

<!-- [ ] 近々論文にしたい手法（論文をサブミットしたら公開する）-->
 
