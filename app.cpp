/*
 *	app.cpp
 *
 *	dplayout.exe - レイアウト定義作成ツール
 *	Copyright (C) 2016 Naoyuki Sawa
 *
 *	* Tue Jun 21 21:15:30 JST 2016 Naoyuki Sawa
 *	- 1st リリース。
 *	* Wed Jun 22 22:49:18 JST 2016 Naoyuki Sawa
 *	- "x","y","w","h"キーの値に、計算式を書けるようにしました。
 *	  ShuntingYard_Eval()を利用しているので、使用出来る演算子についてはShuntingYard_Eval()を参照して下さい。
 *	- 子階層のレイアウト定義を、親階層のレイアウト定義よりも、後で出力するようにしました。
 *	  意味は変わりませんが、この順序の方が見た目が判り易いと思うからです。
 *	* Wed Jun 22 23:18:16 JST 2016 Naoyuki Sawa
 *	- YAMLドキュメントで指定された子階層の座標を、親階層の座標によってオフセットするように変更しました。
 *	  これまでは子階層の座標も絶対座標と見なしていましたが、親階層からの相対座標と見なす方が使い易いと思ったからです。
 *	  最終的に出力されるレイアウト定義は、計算済みの絶対座標となります。
 *	* Sat Jun 25 21:55:36 JST 2016 Naoyuki Sawa
 *	- 計算式の処理を、これまではShuntingYard_Eval()を使用していましたが、expreval()を使用するように変更しました。
 *	- '-D'オプションを追加しました。
 */
#include "app.h"
#define VERSION		"20160625"	//最終更新日
/****************************************************************************
 *	main
 ****************************************************************************/
int main(int argc, char* argv[]) {
	int exitCode = EXIT_SUCCESS;
#ifdef  _DEBUG
	remove("syslog.out");			//毎回ログを削除する事にした。
    #ifndef USE_JOB_DEBUG
	openlog(NULL, LOG_PERROR, LOG_USER);	//デバッグビルドでは、システムログを標準エラーにも出力する。ただし、デバッグジョブを使用する場合は、デバッグジョブのcurses表示と衝突しないよう、標準エラーには出力しない。
    #endif//USE_JOB_DEBUG
#else //_DEBUG
	setlogmask(LOG_UPTO(LOG_INFO));		//リリースビルドでは、LOG_DEBUGを抑制する。
#endif//_DEBUG
#if     (defined(GC_H) && !defined(USE_BISON_FLEX))
	putenv("GC_LOG_FILE=CON");//ログ出力先を指定する。
	GC_set_find_leak(1);//メモリリーク検出を開始する。
#endif//(defined(GC_H) && !defined(USE_BISON_FLEX))
#ifdef  __CLIP_SEH_H__
	SEH_init();
#endif//__CLIP_SEH_H__
#ifdef  USE_GDIPLUS
	Gdiplus_Init();
#endif//USE_GDIPLUS
	app_init(argc, argv);
	exitCode = app_main(argc, argv);
	app_exit();
#ifdef  USE_GDIPLUS
	Gdiplus_Exit();
#endif//USE_GDIPLUS
#if     (defined(GC_H) && !defined(USE_BISON_FLEX))
	CHECK_LEAKS();//メモリリークを検出する。
#endif//(defined(GC_H) && !defined(USE_BISON_FLEX))
	return exitCode;
}
/****************************************************************************
 *	usage
 ****************************************************************************/
__declspec(noreturn) static void usage() {
	fprintf(stderr, "dplayout.exe - レイアウト定義作成ツール (%s)\n", VERSION);
	fprintf(stderr, "Copyright (C) 2016 Naoyuki Sawa\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "USAGE:\n");
	fprintf(stderr, "  dpsprsht.exe [option ...] YAMLファイル名 [YAMLファイル名 ...]\n");
	fprintf(stderr, "  ・YAMLファイル名は、ワイルドカードによる複数指定も可能です。\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "OPTIONS:\n");
	fprintf(stderr, "  -D変数名=値[,...] 計算式の中で参照する変数を定義します。\n");
	fprintf(stderr, "                    複数の変数を定義するには、'-D'オプションを複数回指定するか、\n");
	fprintf(stderr, "                    又は、1つの'-D'オプションの中で','で区切って指定して下さい。\n");
	fprintf(stderr, "  -o出力ファイル名  レイアウト定義の出力ファイル名を指定します。\n");
	fprintf(stderr, "                    このオプションを指定しなければ、標準出力へ出力します。\n");
	fprintf(stderr, "  -v                詳細な警告表示を有効にします。\n");
	fprintf(stderr, "  -h                使い方を表示します。\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "EXAMPLE:\n");
	fprintf(stderr, "  dplayout.exe -v -olayout.h -DDISP_X=128,DISP_Y=88 sample1.yml sample2.yml\n");
	fprintf(stderr, "  詳細な警告表示を有効にします。\n");
	fprintf(stderr, "  sample1.ymlとsample2.ymlを読み込んで、レイアウト定義をlayout.hへ出力します。\n");
	fprintf(stderr, "  計算式の中で参照する変数として、DISP_XとDISP_Yを定義します。\n");
	exit(EXIT_FAILURE);
}
/****************************************************************************
 *	
 ****************************************************************************/
void app_init(int argc, char* argv[]) {
//{{アプリケーション特有の処理
	{
		/** no job **/
	}
//}}アプリケーション特有の処理
}
/*--------------------------------------------------------------------------*/
void app_exit() {
//{{アプリケーション特有の処理
	{
		/** no job **/
	}
//}}アプリケーション特有の処理
}
/*--------------------------------------------------------------------------*/
static double fnGetVar(const char* name) {
	double* pd = (double*)g_dataset_get_data(fnGetVar, name);
	if(pd) { return *pd; }
//削除	//'-v'オプションが指定されていたら…	⇒このメッセージは、'-v'オプションが指定されていなくても、常に表示する事にした。
//削除	if(opt_v) {
		fprintf(stderr, "未定義の変数'%s'が参照されました。\n", name);
//削除	}
	SEH_throw(ExprException);
}
/*--------------------------------------------------------------------------*/
static void process_node(FILE* fp, const char* fname, yaml_document_t* d, yaml_mapping_t* m, char** v, int x0, int y0) {
	int x = 0, x_valid = 0;		//"x"キーを見つけたら、xに値を格納して、x_validを1にセットする。
	int y = 0, y_valid = 0;		//"y"キーを見つけたら、yに値を格納して、y_validを1にセットする。
	int w = 0, w_valid = 0;		//"w"キーを見つけたら、wに値を格納して、w_validを1にセットする。
	int h = 0, h_valid = 0;		//"h"キーを見つけたら、hに値を格納して、h_validを1にセットする。
	const char* img = NULL;		//"img"キーを見つけたら、imgに文字列を格納する。
	//エラーメッセージを表示する時のために、"."で連結したパス名を作成しておく。
	char* err_path = strjoinv(".", v);
	//子階層のキーの名前を記憶しておくための、ポインタ配列を作成する。
	GPtrArray* children = g_ptr_array_new();
	//マッピングの各キーについて…
	int i, c = yaml_mapping_get_count(d, m);
	for(i = 0; i < c; i++) {
		//キーの名前を取得する。
		const char* k = yaml_mapping_get_key(d, m, i);
		//キーの値のノードを取得する。
		yaml_node_t* n = yaml_mapping_get_node(d, m, k);
		//キーの値の種類によって…
		switch(yaml_node_get_type(n)) {
		default:DIE();
		//キーの値がスカラーならば…
		case YAML_SCALAR_NODE:
			{
				//キーの名前が"x"ならば…
				if(!strcmp(k, "x")) {
					//"x"キーが重複していない事を確認する。
					if(x_valid) {
						error(EXIT_FAILURE, 0, "%s %s.%s キーが重複しています。", fname, err_path, k);
					}
					//"x"キーの値を取得する。
					SEH_try {
						x = expreval(yaml_node_get_string(n), fnGetVar, NULL);
					} SEH_catch(ExprException) {
						error(EXIT_FAILURE, 0, "%s %s.%s キーの値が不正です。", fname, err_path, k);
					} SEH_end
					//"x"キーの値を取得した事をマークする。
					x_valid = 1;
				//キーの名前が"y"ならば…
				} else if(!strcmp(k, "y")) {
					//"y"キーが重複していない事を確認する。
					if(y_valid) {
						error(EXIT_FAILURE, 0, "%s %s.%s キーが重複しています。", fname, err_path, k);
					}
					//"y"キーの値を取得する。
					SEH_try {
						y = expreval(yaml_node_get_string(n), fnGetVar, NULL);
					} SEH_catch(ExprException) {
						error(EXIT_FAILURE, 0, "%s %s.%s キーの値が不正です。", fname, err_path, k);
					} SEH_end
					//"y"キーの値を取得した事をマークする。
					y_valid = 1;
				//キーの名前が"w"ならば…
				} else if(!strcmp(k, "w")) {
					//"w"キーが重複していない事を確認する。
					if(w_valid) {
						error(EXIT_FAILURE, 0, "%s %s.%s キーが重複しています。", fname, err_path, k);
					}
					//"w"キーの値を取得する。
					SEH_try {
						w = expreval(yaml_node_get_string(n), fnGetVar, NULL);
					} SEH_catch(ExprException) {
						error(EXIT_FAILURE, 0, "%s %s.%s キーの値が不正です。", fname, err_path, k);
					} SEH_end
					//"w"キーの値を取得した事をマークする。
					w_valid = 1;
				//キーの名前が"h"ならば…
				} else if(!strcmp(k, "h")) {
					//"h"キーが重複していない事を確認する。
					if(h_valid) {
						error(EXIT_FAILURE, 0, "%s %s.%s キーが重複しています。", fname, err_path, k);
					}
					//"h"キーの値を取得する。
					SEH_try {
						h = expreval(yaml_node_get_string(n), fnGetVar, NULL);
					} SEH_catch(ExprException) {
						error(EXIT_FAILURE, 0, "%s %s.%s キーの値が不正です。", fname, err_path, k);
					} SEH_end
					//"h"キーの値を取得した事をマークする。
					h_valid = 1;
				//キーの名前が"img"ならば…
				} else if(!strcmp(k, "img")) {
					//"img"キーが重複していない事を確認する。
					if(img) {
						error(EXIT_FAILURE, 0, "%s %s.%s キーが重複しています。", fname, err_path, k);
					}
					//"img"キーの文字列を取得する。
					img = yaml_node_get_string(n);
				//キーの名前が上記以外ならば…
				} else {
					error(EXIT_FAILURE, 0, "%s %s.%s キーの名前が不正です。", fname, err_path, k);
				}
			}
			break;
		//キーの値がシーケンスならば…
		case YAML_SEQUENCE_NODE:
			{
				//当ツールの入力ファイルとなるYAMLドキュメントでは、シーケンスを使用してはいけない。
				error(EXIT_FAILURE, 0, "%s %s シーケンスを使用してはいけません。", fname, err_path);
			}
		//キーの値がマッピングならば…
		case YAML_MAPPING_NODE:
			{
				//子階層のキーの名前を記憶しておく。
				g_ptr_array_add(children, (void*)k);	//キャストはconst警告抑制のため。
			}
			break;
		}
	}
	//画像ファイル名が指定されており、かつ、"w","h"キーの一方でも指定されていなければ、画像から取得を試みる。
	if(img && (!w_valid || !h_valid)) {
		//画像ファイル名を、Unicode文字列に変換する。
		wchar_t* uni = strdup_ShiftJisToUnicode(img);
		//画像を読み込む。
		Image* image = new Image(uni);
		//画像が読み込めたら…
		if(!image->GetLastStatus()) {
			//"w"キーが指定されていなければ…
			if(!w_valid) {
				//画像から、"w"キーの値を取得する。
				w = image->GetWidth();
				//"w"キーの値を取得した事をマークする。
				w_valid = 1;
				//'-v'オプションが指定されていたら…
				if(opt_v) {
					//"w"キーの値を、画像から取得した事を表示する。
					fprintf(stderr, "%s %s.%s キーの値を、画像から取得しました。\n", fname, err_path, "w");
				}
			}
			//"h"キーが指定されていなければ…
			if(!h_valid) {
				//画像から、"h"キーの値を取得する。
				h = image->GetHeight();
				//"h"キーの値を取得した事をマークする。
				h_valid = 1;
				//'-v'オプションが指定されていたら…
				if(opt_v) {
					//"h"キーの値を、画像から取得した事を表示する。
					fprintf(stderr, "%s %s.%s キーの値を、画像から取得しました。\n", fname, err_path, "h");
				}
			}
		//画像が読み込めなければ…
		} else {
//削除			//'-v'オプションが指定されていたら…	⇒このメッセージは、'-v'オプションが指定されていなくても、常に表示する事にした。
//削除			if(opt_v) {
				//警告を表示し、処理は継続する。
				fprintf(stderr, "警告: %s 画像ファイル %s が読み込めません。\n", fname, img);
//削除			}
		}
		//画像を解放する。
		delete image;
		//画像ファイル名の、Unicode文字列を解放する。
		free(uni);
	}
	//"x","y","w","h"キーの内、一つでも指定させていたら…
	if(x_valid || y_valid || w_valid || h_valid) {
		//ルートノードでなければ…
		if(v[0]) {
			//シンボル定義のための、"__"で連結したパスを作成する。
			char* def_path = strjoinv("__", v);
			//シンボル定義を出力する。
			fprintf(fp, "//%s\n", err_path/*コメントに出力するパスは、"__"区切りよりも"."区切りの方が見易いと思ったので、def_pathではなくerr_pathを使う事にした。*/);
			if(x_valid) { fprintf(fp, "#define X__%s\t%d\n", def_path, x + x0); }
			if(y_valid) { fprintf(fp, "#define Y__%s\t%d\n", def_path, y + y0); }
			if(w_valid) { fprintf(fp, "#define W__%s\t%d\n", def_path, w); }
			if(h_valid) { fprintf(fp, "#define H__%s\t%d\n", def_path, h); }
			//"__"で連結したパスを解放する。
			free(def_path);
		//ルートノードならば…
		} else {
			//ルートノードにレイアウトを定義してはいけない。
			error(EXIT_FAILURE, 0, "%s ルートノードにレイアウトを定義してはいけません。", fname);
		}
	}
	//全ての子階層について…
	while(children->len) {
		//子階層のキーの名前を取得する。
		const char* k = (const char*)g_ptr_array_remove_index(children, 0);
		//キーの値のノードを取得する。
		yaml_node_t* n = yaml_mapping_get_node(d, m, k);
		//パスの末尾にこのキーの名前を追加した、一時的なリストを作成する。
		char** v2 = strv_append(v, k);
		//再帰処理を行う。
		process_node(fp, fname, d, yaml_node_get_mapping(n), v2, x + x0, y + y0);	//もし(!x_valid)ならば(x==0)なので、x_validを検査せずに加算して大丈夫です。y_validとyについても同様です。
		//一時的なリストを解放する。
		strv_free(v2);
	}
	//子階層のキーの名前を記憶していた、ポインタ配列を開放する。
	g_ptr_array_unref(children);
	//"."で連結したパス名を解放する。
	free(err_path);
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int app_main(int argc, char* argv[]) {
	{
		//コマンドラインオプションを処理する。
		int opt;
		opterr = 0; //オプションエラーは当プログラム内で処理するので、getopt()内でのエラー表示は不要。
		while((opt = getopt(argc, argv, "D:o:vh")) != -1) {	//コマンドラインオプションを処理する。
			switch(opt) {
			case 'D':
				do {
					char* name = strsep(&optarg, "=");		//「name=value,name=value,...」の、最初の'name'を取得する。
					if(!name || !*name) { usage(); }		//nameが空文字ならばエラー。
					char* value = strsep(&optarg, ",");		//「value,name=value,...」の、最初の'value'を取得する。
					if(!value || !*value) { usage(); }		//valueが空文字ならばエラー。
					double* pd = (double*)malloc(sizeof(double));	//数値をデータセットに登録するためのメモリを確保する。
					if(!pd) { DIE(); }				//メモリを確保出来なければエラー。
					*pd = strtod(value, &value);			//valueの数値を取得する。
					if(*value) { usage(); }				//数値が不正ならばエラー。
					g_dataset_set_data(fnGetVar, name, pd);		//数値をデータセットに登録する。
				} while(optarg);
				break;
			case 'o':
				opt_o = optarg;
				break;
			case 'v':
				opt_v = 1;
				break;
			case 'h':
			default:
				usage();
			}
		}
		//少なくとも一つ以上、入力ファイルが指定されていなければいけない。
		if(optind >= argc) { usage(); }
	}
	{
		FILE* fp = stdout;	//'-o'オプションが指定されていなければ、標準出力へ出力する。
		//'-o'オプションが指定されていたら…
		if(opt_o) {
			//出力ファイルを作成する。
			fp = fopen(opt_o, "w");
			if(!fp) {
				error(EXIT_FAILURE, errno, "%s が作成出来ません。", opt_o);
			}
		}
		//各入力ファイル(YAMLドキュメント)を処理する。
		do {
			const char* fname = argv[optind];
			yaml_document_t* d;
			yaml_mapping_t* m;
			char** v;
			//YAMLドキュメントを読み込む。
			SEH_try {
				d = yaml_document_load(fname);
			} SEH_catch(YamlException) {
				error(EXIT_FAILURE, 0, "%s が読み込めません。", fname);
			} SEH_end
			//ルートノードを取得する。
			SEH_try {
				m = yaml_document_get_root_mapping(d);
			} SEH_catch(YamlException) {
				error(EXIT_FAILURE, 0, "%s ルートノードはマッピングでなくてはなりません。", fname);
			} SEH_end
			//空のパスリストを作成する。
			v = strv_new(NULL);
			//再帰処理を行う。
			process_node(fp, fname, d, m, v, 0, 0);
			//空のパスリストを解放する。
			strv_free(v);
			//YAMLドキュメントを解放する。
			yaml_document_free(d);
		} while(++optind < argc);
		//'-o'オプションが指定されていたら…
		if(opt_o) {
			//出力ファイルを閉じる。
			fclose(fp);
		}
	}
	return EXIT_SUCCESS;
}
