# PIREVERB.X

On-demand reverb type controller for ras68k-ext on Human68k/X680x0

---

## About This

X680x0 のパラレルインターフェイスと Raspberry Pi + Pico を活用したレトロ音源システムである ras68k-ext 専用の常駐型リバーブタイプ切り替えツールです。

- キーボードのホットキーでいつでも ras68k-ext のリバーブタイプを8種類から切り替え可能

ras68k-ext の詳細については、開発者である opmregisters氏のサイトを参照してください。

技術資料
* [http://opmregisters.web.fc2.com/ras68k/](http://opmregisters.web.fc2.com/ras68k/)

BOOTH
* [https://booth.pm/ja/items/1178236](https://booth.pm/ja/items/1178236)


注意：本プログラムは 2023年5月より頒布の始まった、Raspberry Pi Picoを搭載した新バージョンのras68k-extシステムのみを想定しています。(前バージョンについてはハードウェアを所有しておらず検証不可能なため)

注意：公式ドライバのPILIB.Xでも同様の機能のサポートがアナウンスされていますので、このプログラムはあくまで繋ぎの位置付けです。

---

## Install

PIRVBxxx.ZIP をダウンロードして、PIREVERB.X をパスの通ったディレクトリにコピーします。

---

### 割り込みに関して

このプログラムは Timer-D の割り込みを使用します。これらの割り込みを使う常駐プログラムなどと一緒に動作させることはできません。

また、Timer-Dを使うHuman68k のバックグラウンド機能も利用できません。CONFIG.SYS で PROCESS= の行が有効になっている場合は、本プログラム利用時はコメントアウトしておく必要があります。

---

## Usage

注意：本プログラムの動作には ras68k-ext サポートライブラリ兼ドライバである PILIB.X の導入と常駐が必要になります。PILIB.X が常駐していない場合はエラーとなり起動できません。

引数をつけずに実行するか、`-h` オプションをつけて実行するとヘルプメッセージが表示されます。

    usage: pireverb [options]
    options:
         -r    ... remove program 常駐解除します
         -t[n] ... 常駐時に設定するリバーブタイプを指定します。デフォルトは 2.STUDIO SMALL です
         -q    ... quiet mode リバーブタイプ切り替え時にメッセージを表示しません
         -h    ... show help message ヘルプメッセージを表示します

常駐後は OPT.2 + XF1 を同時に押すことでリバーブタイプを切り替えることができます。`-q` を指定しなかった場合は、テキスト画面の最下段にどのリバーブタイプに切り替えたのかの表示を行います。

なぜそんな中途半端なキー配置にしているかと言うと、MXDRV自体がOPT.1+XF3/4/5あたりを元々再生制御に使っているためです。

また、OPT.2 + XF2 を同時に押すと、現在のリバーブタイプの表示のみ行います。(`-q`指定時を除く)



https://github.com/tantanGH/pireverb/assets/121137457/d70cd5e6-1959-46c9-a9d6-6d1a870a99b0



---

## 動作確認環境

以下の環境でのみ動作確認しています。

* X68000XVI (MC68000 16.7MHz, 8MB RAM, ArdSCSino-stm32)
* Raspberry Pi 4B + ras68k-ext (2023新バージョン)
* PILIB.X (20221130版)
* ras68k-ext ミドルウェア (20230512版)

本プログラムに関して、ras68k-ext作者の opmregisters氏に問い合わせを行うことはご迷惑になりますのでおやめ下さい。
何かあれば tantan (twitter:@snakGH) までご連絡下さい。

---

## ras68k-ext の PILIB.X ドライバと TwentyOne.X / FASTOPEN.X に関して(非公式)

ras68k-ext 制御ドライバの PILIB.X は PCMデータを既にラズパイ側にアップロード済みかを0バイトのローカルファイルを作ることで管理しています。そのファイル名は 8 + 1 + 8 = 17文字あり、そのすべてが識別に使われます。このため、TwentyOne.X は必ず常駐しておく必要があります。`+T`オプション(21文字識別,デフォルト有効) は必須となります。それ以外の大文字小文字区別や、マルチピリオドを有効にする必要はありません。

また、SHARP純正のファイルオープン高速化ドライバ FASTOPEN.X はTwentyOne.Xととても相性が悪く、これが導入されているとPILIB.Xのキャッシュファイル操作が期待通りに行われません。FASTOPEN.XはCONFIG.SYSから必ず外しておくようにしてください。

---

## ras68k-ext の発熱低減設定に関して(非公式)

ras68k-ext はその機能の豊富さ故、Raspberry Pi上でミドルウェアpcmd(pi68k-ext)を実行するとCPUパワーをかなり消費します。Raspberry Pi 4Bで動かした場合、ラズパイ本体が相当発熱します。

これを抑えるため、当方ではラズパイのCPUクロックを落とす設定をしています。具体的には`/boot/config.txt`の末尾に以下を加えることで、定格の約半分のクロックでの動作としています。

        arm_freq=350
        over_voltage=-4

この状態でも自分の利用する機能(OPMエミュレーションおよびADPCM/PCMエミュレーション)を利用する限りにおいては特に問題は出ていません。
ただし、繰り返しになりますが非公式なものにつきras68k-ext作者のopmregisters氏への問い合わせなどはご遠慮下さい。

なお、`/boot/config.txt`を編集する際は、せっかくなので以下のオーディオ有効化設定がちゃんと存在しコメントアウトされていないか確認しとくと良いでしょう。

        dtparam=audio=on

---

## ras68k-ext 対応 RCD.X の改良版(非公式)

ras68k-ext 公式サイトで配布されている RCD.X の ras68k-ext 対応版である RCD_R.X は残念ながらMPU68030以上専用となっており、000機ではエラーとなり起動できません。また、030であってもPILIB.X常駐前に実行すると白帯が出たり、常駐解除時にSCC割り込みベクタの戻しが正しく行えていません。

これらの問題に対応した非公式の RCD_RAS.X を本サイトからダウンロードできます。

[rcd_ras.zip](https://github.com/tantanGH/pireverb/raw/main/rcd/rcd_ras.zip)

導入・実行の手順は以下の通りです。なお RS232Cポートへのデータ書き出し部分だけを乗っ取る形になっているため、SCC(シリアル通信コントローラ)の設定変更などはそのままSCCに反映されますので注意してください。他のRS-232Cを使うドライバなどと混在させることはできません。

* 事前に PILIB.X を常駐させておく。(常駐していない場合はエラーとなり起動できません)
* RCD_RAS.X -c としてRS-MIDIモード扱いで常駐させる

これは opmregisters氏のRCDパッチを元に作成した非公式なものです。期待通り動作しない・しなくなる可能性がありますので利用は自己責任でお願いします。

---

## Special Thanks

* ras68k-ext ハードウェア, PILIB.X および技術資料 / opmregistersさん
* xdev68k thanks to ファミべのよっしんさん
* HAS060.X on run68mac thanks to YuNKさん / M.Kamadaさん / GOROmanさん
* HLK301.X on run68mac thanks to SALTさん / GOROmanさん

---

## History

* 0.1.0 (2023/05/21) ... 初版

---
