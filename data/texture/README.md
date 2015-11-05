ここには惑星に貼り付けるテクスチャファイルを配置します．

32bitのrawファイルが利用可能で，そのファイル名をWorldファイルに記述します．
ファイル名は'*_{width}x{height}.raw'という形式で画像サイズを指定する必要があり，例えば
> Earth_1000x500.raw
のようになります．

このプログラムの作成時には[Planet Texture Map Collection](http://planetpixelemporium.com/planets.html)のものを利用させていただきました．  
ここではテクスチャがjpgで入手可能ですが，このプログラムでの利用はImageMagickで  
`convert source.jpg rgba:dest_WxH.raw`  
として変換することで可能です．もちろん，GIMPなどのアプリケーションで変換することも可能です．