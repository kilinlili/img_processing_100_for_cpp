# 画像処理100本ノック
C++学習用

## 環境

OpenCV3 (3.2.0)
ubuntu 18.04 (WSL1)
pkg-config

## その他

画像境界の処理は基本的に無視．お任せ．
OpenCVのデフォルトは折り返し表示．
https://qiita.com/fukushima1981/items/cb74323e13f3a26cb67d
らしいのでそれに任せた．

## 問題に関するコメント

Q.33 lowpassフィルタの実装が怪しい  
画素値が1下がるだけという謎実装になってる．  
dftとidft部分は多分問題なし．  

Q.36,37,38,40はパス


