/*
 *	app.cpp
 *
 *	dplayout.exe - ���C�A�E�g��`�쐬�c�[��
 *	Copyright (C) 2016 Naoyuki Sawa
 *
 *	* Tue Jun 21 21:15:30 JST 2016 Naoyuki Sawa
 *	- 1st �����[�X�B
 *	* Wed Jun 22 22:49:18 JST 2016 Naoyuki Sawa
 *	- "x","y","w","h"�L�[�̒l�ɁA�v�Z����������悤�ɂ��܂����B
 *	  ShuntingYard_Eval()�𗘗p���Ă���̂ŁA�g�p�o���鉉�Z�q�ɂ��Ă�ShuntingYard_Eval()���Q�Ƃ��ĉ������B
 *	- �q�K�w�̃��C�A�E�g��`���A�e�K�w�̃��C�A�E�g��`�����A��ŏo�͂���悤�ɂ��܂����B
 *	  �Ӗ��͕ς��܂��񂪁A���̏����̕��������ڂ�����Ղ��Ǝv������ł��B
 *	* Wed Jun 22 23:18:16 JST 2016 Naoyuki Sawa
 *	- YAML�h�L�������g�Ŏw�肳�ꂽ�q�K�w�̍��W���A�e�K�w�̍��W�ɂ���ăI�t�Z�b�g����悤�ɕύX���܂����B
 *	  ����܂ł͎q�K�w�̍��W����΍��W�ƌ��Ȃ��Ă��܂������A�e�K�w����̑��΍��W�ƌ��Ȃ������g���Ղ��Ǝv��������ł��B
 *	  �ŏI�I�ɏo�͂���郌�C�A�E�g��`�́A�v�Z�ς݂̐�΍��W�ƂȂ�܂��B
 *	* Sat Jun 25 21:55:36 JST 2016 Naoyuki Sawa
 *	- �v�Z���̏������A����܂ł�ShuntingYard_Eval()���g�p���Ă��܂������Aexpreval()���g�p����悤�ɕύX���܂����B
 *	- '-D'�I�v�V������ǉ����܂����B
 */
#include "app.h"
#define VERSION		"20160625"	//�ŏI�X�V��
/****************************************************************************
 *	main
 ****************************************************************************/
int main(int argc, char* argv[]) {
	int exitCode = EXIT_SUCCESS;
#ifdef  _DEBUG
	remove("syslog.out");			//���񃍃O���폜���鎖�ɂ����B
    #ifndef USE_JOB_DEBUG
	openlog(NULL, LOG_PERROR, LOG_USER);	//�f�o�b�O�r���h�ł́A�V�X�e�����O��W���G���[�ɂ��o�͂���B�������A�f�o�b�O�W���u���g�p����ꍇ�́A�f�o�b�O�W���u��curses�\���ƏՓ˂��Ȃ��悤�A�W���G���[�ɂ͏o�͂��Ȃ��B
    #endif//USE_JOB_DEBUG
#else //_DEBUG
	setlogmask(LOG_UPTO(LOG_INFO));		//�����[�X�r���h�ł́ALOG_DEBUG��}������B
#endif//_DEBUG
#if     (defined(GC_H) && !defined(USE_BISON_FLEX))
	putenv("GC_LOG_FILE=CON");//���O�o�͐���w�肷��B
	GC_set_find_leak(1);//���������[�N���o���J�n����B
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
	CHECK_LEAKS();//���������[�N�����o����B
#endif//(defined(GC_H) && !defined(USE_BISON_FLEX))
	return exitCode;
}
/****************************************************************************
 *	usage
 ****************************************************************************/
__declspec(noreturn) static void usage() {
	fprintf(stderr, "dplayout.exe - ���C�A�E�g��`�쐬�c�[�� (%s)\n", VERSION);
	fprintf(stderr, "Copyright (C) 2016 Naoyuki Sawa\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "USAGE:\n");
	fprintf(stderr, "  dpsprsht.exe [option ...] YAML�t�@�C���� [YAML�t�@�C���� ...]\n");
	fprintf(stderr, "  �EYAML�t�@�C�����́A���C���h�J�[�h�ɂ�镡���w����\�ł��B\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "OPTIONS:\n");
	fprintf(stderr, "  -D�ϐ���=�l[,...] �v�Z���̒��ŎQ�Ƃ���ϐ����`���܂��B\n");
	fprintf(stderr, "                    �����̕ϐ����`����ɂ́A'-D'�I�v�V�����𕡐���w�肷�邩�A\n");
	fprintf(stderr, "                    ���́A1��'-D'�I�v�V�����̒���','�ŋ�؂��Ďw�肵�ĉ������B\n");
	fprintf(stderr, "  -o�o�̓t�@�C����  ���C�A�E�g��`�̏o�̓t�@�C�������w�肵�܂��B\n");
	fprintf(stderr, "                    ���̃I�v�V�������w�肵�Ȃ���΁A�W���o�͂֏o�͂��܂��B\n");
	fprintf(stderr, "  -v                �ڍׂȌx���\����L���ɂ��܂��B\n");
	fprintf(stderr, "  -h                �g������\�����܂��B\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "EXAMPLE:\n");
	fprintf(stderr, "  dplayout.exe -v -olayout.h -DDISP_X=128,DISP_Y=88 sample1.yml sample2.yml\n");
	fprintf(stderr, "  �ڍׂȌx���\����L���ɂ��܂��B\n");
	fprintf(stderr, "  sample1.yml��sample2.yml��ǂݍ���ŁA���C�A�E�g��`��layout.h�֏o�͂��܂��B\n");
	fprintf(stderr, "  �v�Z���̒��ŎQ�Ƃ���ϐ��Ƃ��āADISP_X��DISP_Y���`���܂��B\n");
	exit(EXIT_FAILURE);
}
/****************************************************************************
 *	
 ****************************************************************************/
void app_init(int argc, char* argv[]) {
//{{�A�v���P�[�V�������L�̏���
	{
		/** no job **/
	}
//}}�A�v���P�[�V�������L�̏���
}
/*--------------------------------------------------------------------------*/
void app_exit() {
//{{�A�v���P�[�V�������L�̏���
	{
		/** no job **/
	}
//}}�A�v���P�[�V�������L�̏���
}
/*--------------------------------------------------------------------------*/
static double fnGetVar(const char* name) {
	double* pd = (double*)g_dataset_get_data(fnGetVar, name);
	if(pd) { return *pd; }
//�폜	//'-v'�I�v�V�������w�肳��Ă�����c	�˂��̃��b�Z�[�W�́A'-v'�I�v�V�������w�肳��Ă��Ȃ��Ă��A��ɕ\�����鎖�ɂ����B
//�폜	if(opt_v) {
		fprintf(stderr, "����`�̕ϐ�'%s'���Q�Ƃ���܂����B\n", name);
//�폜	}
	SEH_throw(ExprException);
}
/*--------------------------------------------------------------------------*/
static void process_node(FILE* fp, const char* fname, yaml_document_t* d, yaml_mapping_t* m, char** v, int x0, int y0) {
	int x = 0, x_valid = 0;		//"x"�L�[����������Ax�ɒl���i�[���āAx_valid��1�ɃZ�b�g����B
	int y = 0, y_valid = 0;		//"y"�L�[����������Ay�ɒl���i�[���āAy_valid��1�ɃZ�b�g����B
	int w = 0, w_valid = 0;		//"w"�L�[����������Aw�ɒl���i�[���āAw_valid��1�ɃZ�b�g����B
	int h = 0, h_valid = 0;		//"h"�L�[����������Ah�ɒl���i�[���āAh_valid��1�ɃZ�b�g����B
	const char* img = NULL;		//"img"�L�[����������Aimg�ɕ�������i�[����B
	//�G���[���b�Z�[�W��\�����鎞�̂��߂ɁA"."�ŘA�������p�X�����쐬���Ă����B
	char* err_path = strjoinv(".", v);
	//�q�K�w�̃L�[�̖��O���L�����Ă������߂́A�|�C���^�z����쐬����B
	GPtrArray* children = g_ptr_array_new();
	//�}�b�s���O�̊e�L�[�ɂ��āc
	int i, c = yaml_mapping_get_count(d, m);
	for(i = 0; i < c; i++) {
		//�L�[�̖��O���擾����B
		const char* k = yaml_mapping_get_key(d, m, i);
		//�L�[�̒l�̃m�[�h���擾����B
		yaml_node_t* n = yaml_mapping_get_node(d, m, k);
		//�L�[�̒l�̎�ނɂ���āc
		switch(yaml_node_get_type(n)) {
		default:DIE();
		//�L�[�̒l���X�J���[�Ȃ�΁c
		case YAML_SCALAR_NODE:
			{
				//�L�[�̖��O��"x"�Ȃ�΁c
				if(!strcmp(k, "x")) {
					//"x"�L�[���d�����Ă��Ȃ������m�F����B
					if(x_valid) {
						error(EXIT_FAILURE, 0, "%s %s.%s �L�[���d�����Ă��܂��B", fname, err_path, k);
					}
					//"x"�L�[�̒l���擾����B
					SEH_try {
						x = expreval(yaml_node_get_string(n), fnGetVar, NULL);
					} SEH_catch(ExprException) {
						error(EXIT_FAILURE, 0, "%s %s.%s �L�[�̒l���s���ł��B", fname, err_path, k);
					} SEH_end
					//"x"�L�[�̒l���擾���������}�[�N����B
					x_valid = 1;
				//�L�[�̖��O��"y"�Ȃ�΁c
				} else if(!strcmp(k, "y")) {
					//"y"�L�[���d�����Ă��Ȃ������m�F����B
					if(y_valid) {
						error(EXIT_FAILURE, 0, "%s %s.%s �L�[���d�����Ă��܂��B", fname, err_path, k);
					}
					//"y"�L�[�̒l���擾����B
					SEH_try {
						y = expreval(yaml_node_get_string(n), fnGetVar, NULL);
					} SEH_catch(ExprException) {
						error(EXIT_FAILURE, 0, "%s %s.%s �L�[�̒l���s���ł��B", fname, err_path, k);
					} SEH_end
					//"y"�L�[�̒l���擾���������}�[�N����B
					y_valid = 1;
				//�L�[�̖��O��"w"�Ȃ�΁c
				} else if(!strcmp(k, "w")) {
					//"w"�L�[���d�����Ă��Ȃ������m�F����B
					if(w_valid) {
						error(EXIT_FAILURE, 0, "%s %s.%s �L�[���d�����Ă��܂��B", fname, err_path, k);
					}
					//"w"�L�[�̒l���擾����B
					SEH_try {
						w = expreval(yaml_node_get_string(n), fnGetVar, NULL);
					} SEH_catch(ExprException) {
						error(EXIT_FAILURE, 0, "%s %s.%s �L�[�̒l���s���ł��B", fname, err_path, k);
					} SEH_end
					//"w"�L�[�̒l���擾���������}�[�N����B
					w_valid = 1;
				//�L�[�̖��O��"h"�Ȃ�΁c
				} else if(!strcmp(k, "h")) {
					//"h"�L�[���d�����Ă��Ȃ������m�F����B
					if(h_valid) {
						error(EXIT_FAILURE, 0, "%s %s.%s �L�[���d�����Ă��܂��B", fname, err_path, k);
					}
					//"h"�L�[�̒l���擾����B
					SEH_try {
						h = expreval(yaml_node_get_string(n), fnGetVar, NULL);
					} SEH_catch(ExprException) {
						error(EXIT_FAILURE, 0, "%s %s.%s �L�[�̒l���s���ł��B", fname, err_path, k);
					} SEH_end
					//"h"�L�[�̒l���擾���������}�[�N����B
					h_valid = 1;
				//�L�[�̖��O��"img"�Ȃ�΁c
				} else if(!strcmp(k, "img")) {
					//"img"�L�[���d�����Ă��Ȃ������m�F����B
					if(img) {
						error(EXIT_FAILURE, 0, "%s %s.%s �L�[���d�����Ă��܂��B", fname, err_path, k);
					}
					//"img"�L�[�̕�������擾����B
					img = yaml_node_get_string(n);
				//�L�[�̖��O����L�ȊO�Ȃ�΁c
				} else {
					error(EXIT_FAILURE, 0, "%s %s.%s �L�[�̖��O���s���ł��B", fname, err_path, k);
				}
			}
			break;
		//�L�[�̒l���V�[�P���X�Ȃ�΁c
		case YAML_SEQUENCE_NODE:
			{
				//���c�[���̓��̓t�@�C���ƂȂ�YAML�h�L�������g�ł́A�V�[�P���X���g�p���Ă͂����Ȃ��B
				error(EXIT_FAILURE, 0, "%s %s �V�[�P���X���g�p���Ă͂����܂���B", fname, err_path);
			}
		//�L�[�̒l���}�b�s���O�Ȃ�΁c
		case YAML_MAPPING_NODE:
			{
				//�q�K�w�̃L�[�̖��O���L�����Ă����B
				g_ptr_array_add(children, (void*)k);	//�L���X�g��const�x���}���̂��߁B
			}
			break;
		}
	}
	//�摜�t�@�C�������w�肳��Ă���A���A"w","h"�L�[�̈���ł��w�肳��Ă��Ȃ���΁A�摜����擾�����݂�B
	if(img && (!w_valid || !h_valid)) {
		//�摜�t�@�C�������AUnicode������ɕϊ�����B
		wchar_t* uni = strdup_ShiftJisToUnicode(img);
		//�摜��ǂݍ��ށB
		Image* image = new Image(uni);
		//�摜���ǂݍ��߂���c
		if(!image->GetLastStatus()) {
			//"w"�L�[���w�肳��Ă��Ȃ���΁c
			if(!w_valid) {
				//�摜����A"w"�L�[�̒l���擾����B
				w = image->GetWidth();
				//"w"�L�[�̒l���擾���������}�[�N����B
				w_valid = 1;
				//'-v'�I�v�V�������w�肳��Ă�����c
				if(opt_v) {
					//"w"�L�[�̒l���A�摜����擾��������\������B
					fprintf(stderr, "%s %s.%s �L�[�̒l���A�摜����擾���܂����B\n", fname, err_path, "w");
				}
			}
			//"h"�L�[���w�肳��Ă��Ȃ���΁c
			if(!h_valid) {
				//�摜����A"h"�L�[�̒l���擾����B
				h = image->GetHeight();
				//"h"�L�[�̒l���擾���������}�[�N����B
				h_valid = 1;
				//'-v'�I�v�V�������w�肳��Ă�����c
				if(opt_v) {
					//"h"�L�[�̒l���A�摜����擾��������\������B
					fprintf(stderr, "%s %s.%s �L�[�̒l���A�摜����擾���܂����B\n", fname, err_path, "h");
				}
			}
		//�摜���ǂݍ��߂Ȃ���΁c
		} else {
//�폜			//'-v'�I�v�V�������w�肳��Ă�����c	�˂��̃��b�Z�[�W�́A'-v'�I�v�V�������w�肳��Ă��Ȃ��Ă��A��ɕ\�����鎖�ɂ����B
//�폜			if(opt_v) {
				//�x����\�����A�����͌p������B
				fprintf(stderr, "�x��: %s �摜�t�@�C�� %s ���ǂݍ��߂܂���B\n", fname, img);
//�폜			}
		}
		//�摜���������B
		delete image;
		//�摜�t�@�C�����́AUnicode��������������B
		free(uni);
	}
	//"x","y","w","h"�L�[�̓��A��ł��w�肳���Ă�����c
	if(x_valid || y_valid || w_valid || h_valid) {
		//���[�g�m�[�h�łȂ���΁c
		if(v[0]) {
			//�V���{����`�̂��߂́A"__"�ŘA�������p�X���쐬����B
			char* def_path = strjoinv("__", v);
			//�V���{����`���o�͂���B
			fprintf(fp, "//%s\n", err_path/*�R�����g�ɏo�͂���p�X�́A"__"��؂����"."��؂�̕������Ղ��Ǝv�����̂ŁAdef_path�ł͂Ȃ�err_path���g�����ɂ����B*/);
			if(x_valid) { fprintf(fp, "#define X__%s\t%d\n", def_path, x + x0); }
			if(y_valid) { fprintf(fp, "#define Y__%s\t%d\n", def_path, y + y0); }
			if(w_valid) { fprintf(fp, "#define W__%s\t%d\n", def_path, w); }
			if(h_valid) { fprintf(fp, "#define H__%s\t%d\n", def_path, h); }
			//"__"�ŘA�������p�X���������B
			free(def_path);
		//���[�g�m�[�h�Ȃ�΁c
		} else {
			//���[�g�m�[�h�Ƀ��C�A�E�g���`���Ă͂����Ȃ��B
			error(EXIT_FAILURE, 0, "%s ���[�g�m�[�h�Ƀ��C�A�E�g���`���Ă͂����܂���B", fname);
		}
	}
	//�S�Ă̎q�K�w�ɂ��āc
	while(children->len) {
		//�q�K�w�̃L�[�̖��O���擾����B
		const char* k = (const char*)g_ptr_array_remove_index(children, 0);
		//�L�[�̒l�̃m�[�h���擾����B
		yaml_node_t* n = yaml_mapping_get_node(d, m, k);
		//�p�X�̖����ɂ��̃L�[�̖��O��ǉ������A�ꎞ�I�ȃ��X�g���쐬����B
		char** v2 = strv_append(v, k);
		//�ċA�������s���B
		process_node(fp, fname, d, yaml_node_get_mapping(n), v2, x + x0, y + y0);	//����(!x_valid)�Ȃ��(x==0)�Ȃ̂ŁAx_valid�����������ɉ��Z���đ��v�ł��By_valid��y�ɂ��Ă����l�ł��B
		//�ꎞ�I�ȃ��X�g���������B
		strv_free(v2);
	}
	//�q�K�w�̃L�[�̖��O���L�����Ă����A�|�C���^�z����J������B
	g_ptr_array_unref(children);
	//"."�ŘA�������p�X�����������B
	free(err_path);
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int app_main(int argc, char* argv[]) {
	{
		//�R�}���h���C���I�v�V��������������B
		int opt;
		opterr = 0; //�I�v�V�����G���[�͓��v���O�������ŏ�������̂ŁAgetopt()���ł̃G���[�\���͕s�v�B
		while((opt = getopt(argc, argv, "D:o:vh")) != -1) {	//�R�}���h���C���I�v�V��������������B
			switch(opt) {
			case 'D':
				do {
					char* name = strsep(&optarg, "=");		//�uname=value,name=value,...�v�́A�ŏ���'name'���擾����B
					if(!name || !*name) { usage(); }		//name���󕶎��Ȃ�΃G���[�B
					char* value = strsep(&optarg, ",");		//�uvalue,name=value,...�v�́A�ŏ���'value'���擾����B
					if(!value || !*value) { usage(); }		//value���󕶎��Ȃ�΃G���[�B
					double* pd = (double*)malloc(sizeof(double));	//���l���f�[�^�Z�b�g�ɓo�^���邽�߂̃��������m�ۂ���B
					if(!pd) { DIE(); }				//���������m�ۏo���Ȃ���΃G���[�B
					*pd = strtod(value, &value);			//value�̐��l���擾����B
					if(*value) { usage(); }				//���l���s���Ȃ�΃G���[�B
					g_dataset_set_data(fnGetVar, name, pd);		//���l���f�[�^�Z�b�g�ɓo�^����B
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
		//���Ȃ��Ƃ���ȏ�A���̓t�@�C�����w�肳��Ă��Ȃ���΂����Ȃ��B
		if(optind >= argc) { usage(); }
	}
	{
		FILE* fp = stdout;	//'-o'�I�v�V�������w�肳��Ă��Ȃ���΁A�W���o�͂֏o�͂���B
		//'-o'�I�v�V�������w�肳��Ă�����c
		if(opt_o) {
			//�o�̓t�@�C�����쐬����B
			fp = fopen(opt_o, "w");
			if(!fp) {
				error(EXIT_FAILURE, errno, "%s ���쐬�o���܂���B", opt_o);
			}
		}
		//�e���̓t�@�C��(YAML�h�L�������g)����������B
		do {
			const char* fname = argv[optind];
			yaml_document_t* d;
			yaml_mapping_t* m;
			char** v;
			//YAML�h�L�������g��ǂݍ��ށB
			SEH_try {
				d = yaml_document_load(fname);
			} SEH_catch(YamlException) {
				error(EXIT_FAILURE, 0, "%s ���ǂݍ��߂܂���B", fname);
			} SEH_end
			//���[�g�m�[�h���擾����B
			SEH_try {
				m = yaml_document_get_root_mapping(d);
			} SEH_catch(YamlException) {
				error(EXIT_FAILURE, 0, "%s ���[�g�m�[�h�̓}�b�s���O�łȂ��Ă͂Ȃ�܂���B", fname);
			} SEH_end
			//��̃p�X���X�g���쐬����B
			v = strv_new(NULL);
			//�ċA�������s���B
			process_node(fp, fname, d, m, v, 0, 0);
			//��̃p�X���X�g���������B
			strv_free(v);
			//YAML�h�L�������g���������B
			yaml_document_free(d);
		} while(++optind < argc);
		//'-o'�I�v�V�������w�肳��Ă�����c
		if(opt_o) {
			//�o�̓t�@�C�������B
			fclose(fp);
		}
	}
	return EXIT_SUCCESS;
}
