概要
Ｈ８／３０６９Ｆネット対応マイコンＬＡＮボード（完成品）を使用していろいろやります．
何をやるかは決めていません．

参考
"12ステップで作る 組込みOS自作入門"をひと通りこなした次のステップとして，
自分でマイコンボードを理解し使いこなすことを目的としています．

hardware.odsについて
マイコンのデータシートはわかりづらいため，レジスタ設定等のハードウェア情報を
スプレッドシートにまとめ直しました．
現時点で実機確認していません．これから少しずつ確認していく予定です．

開発環境構築
マイコンボードは以下のものです．
	Ｈ８／３０６９Ｆネット対応マイコンＬＡＮボード（完成品）
	http://akizukidenshi.com/catalog/g/gK-01271/
USB-シリアル変換ケーブルは以下のものを使用しました．
	ＵＳＢ－シリアル変換ケーブル　スケルトン
	http://akizukidenshi.com/catalog/g/gM-00720/
以下の書籍を元に環境構築しています．
	http://kozos.jp/books/makeos/
macの環境構築はこちらを参考にしました．
	http://d.hatena.ne.jp/satfy/20101226
以下，実際に構築した時に残したメモです．参考にしてください．
	USBシリアルドライバインストール
		PL2303 USB to Serial Driver for Mac OS X
		http://sourceforge.net/projects/osx-pl2303/
	H8用gcc, binutils, などをダウンロード
		 cd ~/Downloads
		 mkdir src
		 cd src
	※	 curl -O http://www.ring.gr.jp/archives/GNU/binutils/binutils-2.21.tar.bz2
		 curl -O http://www.ring.gr.jp/archives/GNU/gcc/gcc-4.5.2/gcc-4.5.2.tar.bz2
		 curl -O http://www.ring.gr.jp/archives/GNU/gmp/gmp-4.3.2.tar.bz2
		 curl -O http://www.ring.gr.jp/archives/GNU/mpfr/mpfr-2.4.2.tar.bz2
		 curl -O http://www.multiprecision.org/mpc/download/mpc-0.8.2.tar.gz
		 ※上記のリンクが切れている場合はファイルをtarで展開できない
		   手動でダウンロード
	展開
		tar jxvf binutils/binutils-2.21.tar.bz2
		tar jxvf gmp-4.3.2.tar.bz2
		tar jxvf mpfr-2.4.2.tar.bz2
		tar zxvf mpc-0.8.2.tar.gz
		tar jxvf gcc-4.5.2.tar.bz2
	 H8用gcc, binutilsなどのインストール
	 /usr/local/h8-elfにインストールする(要makefile変更)
		binutils
			sudo mkdir /usr/local/h8-elf
			cd binutils-2.21
			./configure --target=h8300-elf --disable-nls --prefix=/usr/local/h8-elf
			make
			sudo make install
		gmpライブラリ
			sudo mkdir /usr/local/gmp
			cd ..
			cd gmp-4.3.2
			./configure --prefix=/usr/local/gmp
			make
			make check
			sudo make install
		mpfrライブラリ
			sudo mkdir /usr/local/mpfr
			cd ..
			cd mpfr-2.4.2
			./configure --prefix=/usr/local/mpfr --with-gmp=/usr/local/gmp
			make
			make check
			sudo make install
		mpcライブラリ
			sudo mkdir /usr/local/mpc
			cd ..
			cd mpc-0.8.2
			./configure --prefix=/usr/local/mpc --with-gmp=/usr/local/gmp --with-mpfr=/usr/local/mpfr
			make
			make check
			sudo make install
		gcc
			cd ..
			mkdir build	※srcフォルダに作る(解凍フォルダの外)。解凍フォルダ内でやるとエラーになる
			cd build
			../gcc-4.5.2/configure --target=h8300-elf --disable-nls --disable-threads --disable-shared --disable-libssp --enable-languages=c --with-gmp=/usr/local/gmp --with-mpfr=/usr/local/mpfr --with-mpc=/usr/local/mpc --prefix=/usr/local/h8-elf	
			make
			sudo make install
	シリアルドライバ認識確認
		シリアルUSBをさして以下のコマンドを実行
		ls -l /dev/cu* 
		      	/dev/cu.PL2303-00004006と表示される(数字はさす場所によって違う)
	h8writeインストール
		cd ..	
		cd ..	Downloadフォルダに移動
		curl -O http://mes.sourceforge.jp/h8/h8write.c
		gcc -Wall -o h8write h8write.c
		~/path/to/this/repos/tools/h8write/	この中に上記ソースとバイナリを移動

makeコマンド
	$ make		#オブジェクトファイルの生成
	$ make image	#実行ファイルの生成
	$ make write	#書込み(SW1 ON ON OFF ON)
	$ make clean	#生成ファイルの削除
minicom
	mode5でボードを起動(SW1 ON OFF ON OFF)
	& sudo minicom -s -o
	ボーレート		:9600bps
	データ長			:8bit
	ストップビット		:1bit
	パリティ			:なし
	ハードウェア・フロー制御	:なし
	ソフトウェア・フロー制御	:なし




