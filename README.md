# SapporoTdZddApps
[TdZdd](https://github.com/kunisura/TdZdd) と [SAPPOROBDD](http://www.lab2.kuis.kyoto-u.ac.jp/minato/SAPPOROBDD/) の橋渡しライブラリ

実装予定の機能は下記の通り
* 各種制約下での DD 構築
  * グラフ制約（[FrontierBasedSearchWithVetexIndices](https://github.com/hs-nazuna/FrontierBasedSearchWithVertexIndices) の焼き直し）
    * [x] s-t パス
    * [x] サイクル
    * [ ] 連結成分系（木、森を含む）
      * [x] 単一成分、木
      * [ ] 複数成分、森
    * [x] 次数制約
    * [x] シュタイナー制約
  * その他の制約
    * [x] 線形不等式（整数のみ）
  * 関連する機能
    * [x] DdStructure <--> ZBDD の変換
    * [x] DDの変数順序に合わせたグラフデータの取得
    * [ ] 辺の順序付けを行う Beam Search（優先度：低）
* SAPPOROBDD の拡張としての新規演算など
  * [x] Power set の作成
  * [x] 特定の部分集合を表すZDDの作成
  * [ ] 特定のアイテム集合に着目したZDDへの変換（グラフ制約のZDDから頂点変数だけ抜き出すときなどに便利）
  * [ ] 特定のアイテムを含む/含まない（OnSet などの別名）
  * [ ] 特定のアイテム間の xor を適用
  * [ ] DisjointJoin（優先度：低）
  * [ ] JointJoin（優先度：低）
* 最適化
  * [ ] DP
* その他の機能
  * [x] DdStructure の解凍
  * [x] ZBDD の解凍
 