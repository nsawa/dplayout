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
 *	* Sun Jun 26 12:14:12 JST 2016 Naoyuki Sawa
 *	- "x","y","w","h"�L�[�̎��̒��ŁA����m�[�h�́A���̃L�[�̒l���Q�Əo����悤�ɂ��܂����B
 *	  <��>
 *	  ��Foo:
 *	  ��  x: y + 1
 *	  ��  y: w + 1
 *	  ��  w: h + 1
 *	  ��  h: 10
 *	  <��>
 *	  ��Bar:
 *	  ��  x: (DISP_X - 10) - w
 *	  ��  y: (DISP_Y - 10) - h
 *	  ��  img: C:\usr\PIECE\HTML\IMAGE\piece_img1.gif
 *	  ��  h: 10
 *	- �{���́A�ʂ̃m�[�h�̃L�[���Q�Əo����悤�ɂ����������̂ł����A�w����@�����G�ɂȂ肻���Ȃ̂ƁA�ی��������Ȃ肻���Ȃ̂ŁA�����܂ł̑Ή��͍s���܂���ł����B
 *	  ����g���Ă݂āA�����K�v�����Ȃ�΁A�@�\�ǉ����������ĉ������B
 *	* Sun Jun 26 13:07:58 JST 2016 Naoyuki Sawa
 *	- ���������[�N�̌��o�𖳌��ɂ��܂����B
 *	  ���c�[���ɂ����ẮAExprException��⑫���Čp������ӏ��ŁA���������[�N�͔������܂��B
 *	  �K�x�[�W�R���N�V������L���ɂ��Ă���̂ŁA�����̖��͗L��܂���B
 */
#include "app.h"
#define VERSION		"20160626"	//�ŏI�X�V��
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
//	putenv("GC_LOG_FILE=CON");//���O�o�͐���w�肷��B
//	GC_set_find_leak(1);//���������[�N���o���J�n����B
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
//	CHECK_LEAKS();//���������[�N�����o����B	//�����c�[���ɂ����ẮAExprException��⑫���Čp������ӏ��ŁA���������[�N�͔������܂��B�K�x�[�W�R���N�V������L���ɂ��Ă���̂ŁA�����̖��͗L��܂���B��
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
static double fnGetVar_global(const char* name) {
	double* pd = (double*)g_dataset_get_data(fnGetVar_global, name);
	if(pd) { return *pd; }
	SEH_throw(ExprException);
}
/*--------------------------------------------------------------------------*/
typedef struct _ST_LocalVar {
	unsigned	x_valid :1;	//x�ɒl���i�[������Ax_valid��1���Z�b�g����B
	unsigned	y_valid :1;	//y�ɒl���i�[������Ay_valid��1���Z�b�g����B
	unsigned	w_valid :1;	//w�ɒl���i�[������Aw_valid��1���Z�b�g����B
	unsigned	h_valid :1;	//h�ɒl���i�[������Ah_valid��1���Z�b�g����B
	double		x, y, w, h;
} ST_LocalVar;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static double fnGetVar_local(const char* name, void* arg) {
	ST_LocalVar* pLocalVar = (ST_LocalVar*)arg;
	if(!strcmp(name, "x") && pLocalVar->x_valid) { return pLocalVar->x; }
	if(!strcmp(name, "y") && pLocalVar->y_valid) { return pLocalVar->y; }
	if(!strcmp(name, "w") && pLocalVar->w_valid) { return pLocalVar->w; }
	if(!strcmp(name, "h") && pLocalVar->h_valid) { return pLocalVar->h; }
	return fnGetVar_global(name);
}
/*--------------------------------------------------------------------------*/
static void process_node(FILE* fp, const char* fname, yaml_document_t* d, yaml_mapping_t* m, char** v, double x0, double y0) {
	const char* x_str = NULL;	//"x"�L�[����������Ax_str�ɕ�������i�[����B
	const char* y_str = NULL;	//"y"�L�[����������Ay_str�ɕ�������i�[����B
	const char* w_str = NULL;	//"w"�L�[����������Aw_str�ɕ�������i�[����B
	const char* h_str = NULL;	//"h"�L�[����������Ah_str�ɕ�������i�[����B
	const char* img = NULL;		//"img"�L�[����������Aimg�ɕ�������i�[����B
	char w_buf[16], h_buf[16];	//�摜����"w","h"�L�[�̒l���擾�����ꍇ�ɁA�����񉻂��邽�߂Ɏg�p����B
	ST_LocalVar localVar = { 0 };
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
					if(x_str) {
						error(EXIT_FAILURE, 0, "%s %s.%s �L�[���d�����Ă��܂��B", fname, err_path, k);
					}
					//"x"�L�[�̕�������擾����B
					x_str = yaml_node_get_string(n);
				//�L�[�̖��O��"y"�Ȃ�΁c
				} else if(!strcmp(k, "y")) {
					//"y"�L�[���d�����Ă��Ȃ������m�F����B
					if(y_str) {
						error(EXIT_FAILURE, 0, "%s %s.%s �L�[���d�����Ă��܂��B", fname, err_path, k);
					}
					//"y"�L�[�̕�������擾����B
					y_str = yaml_node_get_string(n);
				//�L�[�̖��O��"w"�Ȃ�΁c
				} else if(!strcmp(k, "w")) {
					//"w"�L�[���d�����Ă��Ȃ������m�F����B
					if(w_str) {
						error(EXIT_FAILURE, 0, "%s %s.%s �L�[���d�����Ă��܂��B", fname, err_path, k);
					}
					//"w"�L�[�̕�������擾����B
					w_str = yaml_node_get_string(n);
				//�L�[�̖��O��"h"�Ȃ�΁c
				} else if(!strcmp(k, "h")) {
					//"h"�L�[���d�����Ă��Ȃ������m�F����B
					if(h_str) {
						error(EXIT_FAILURE, 0, "%s %s.%s �L�[���d�����Ă��܂��B", fname, err_path, k);
					}
					//"h"�L�[�̕�������擾����B
					h_str = yaml_node_get_string(n);
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
	if(img && (!w_str || !h_str)) {
		//�摜�t�@�C�������AUnicode������ɕϊ�����B
		wchar_t* uni = strdup_ShiftJisToUnicode(img);
		//�摜��ǂݍ��ށB
		Image* image = new Image(uni);
		//�摜���ǂݍ��߂���c
		if(!image->GetLastStatus()) {
			//"w"�L�[���w�肳��Ă��Ȃ���΁c
			if(!w_str) {
				//�摜����A"w"�L�[�̒l���擾����B
				snprintf(w_buf, sizeof w_buf, "%d", image->GetWidth());
				w_str = w_buf;
				//'-v'�I�v�V�������w�肳��Ă�����c
				if(opt_v) {
					//"w"�L�[�̒l���A�摜����擾��������\������B
					fprintf(stderr, "%s %s.%s �L�[�̒l���A�摜����擾���܂����B\n", fname, err_path, "w");
				}
			}
			//"h"�L�[���w�肳��Ă��Ȃ���΁c
			if(!h_str) {
				//�摜����A"h"�L�[�̒l���擾����B
				snprintf(h_buf, sizeof h_buf, "%d", image->GetHeight());
				h_str = h_buf;
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
	if(x_str || y_str || w_str || h_str) {
		//���[�g�m�[�h�łȂ���΁c
		if(v[0]) {
			for(;;) {
				int nSuccess = 0;		//����]���o������A���̕ϐ��𑝂₷�B
				const char* pFailure = NULL;	//����]���G���[������������A�ŏ��ɃG���[�����������L�[�̖��O���i�[����B
				//"x"�L�[�̕����񂪗L��A�܂��L���łȂ���΁A����]�����Ă݂�B
				if(x_str && !localVar.x_valid) {
					SEH_try {
						localVar.x = expreval_r(x_str, fnGetVar_local, NULL, &localVar);
						localVar.x_valid = 1;
						nSuccess++;
					} SEH_catch(ExprException) {
						if(!pFailure) { pFailure = "x"; }
					} SEH_end
				}
				//"y"�L�[�̕����񂪗L��A�܂��L���łȂ���΁A����]�����Ă݂�B
				if(y_str && !localVar.y_valid) {
					SEH_try {
						localVar.y = expreval_r(y_str, fnGetVar_local, NULL, &localVar);
						localVar.y_valid = 1;
						nSuccess++;
					} SEH_catch(ExprException) {
						if(!pFailure) { pFailure = "y"; }
					} SEH_end
				}
				//"w"�L�[�̕����񂪗L��A�܂��L���łȂ���΁A����]�����Ă݂�B
				if(w_str && !localVar.w_valid) {
					SEH_try {
						localVar.w = expreval_r(w_str, fnGetVar_local, NULL, &localVar);
						localVar.w_valid = 1;
						nSuccess++;
					} SEH_catch(ExprException) {
						if(!pFailure) { pFailure = "w"; }
					} SEH_end
				}
				//"h"�L�[�̕����񂪗L��A�܂��L���łȂ���΁A����]�����Ă݂�B
				if(h_str && !localVar.h_valid) {
					SEH_try {
						localVar.h = expreval_r(h_str, fnGetVar_local, NULL, &localVar);
						localVar.h_valid = 1;
						nSuccess++;
					} SEH_catch(ExprException) {
						if(!pFailure) { pFailure = "h"; }
					} SEH_end
				}
				//������s���Ȃ���΁A���[�v�𔲂���B
				if(!pFailure) { break; }
				//��ł����s������A���[�v���p������B
				//�������A����������Ă��Ȃ���΁A�G���[�I������B
				//��̓I�ɂ́A���[�J���ϐ��̑��ݎQ�Ƃ����݂���,����,����`�̃O���[�o���ϐ����Q�Ƃ���Ă���ꍇ�ɁA�����ŃG���[��~����B
				if(!nSuccess) {
					error(EXIT_FAILURE, 0, "%s %s.%s �L�[�̒l���s���ł��B", fname, err_path, pFailure);
				}
			}
			//�V���{����`�̂��߂́A"__"�ŘA�������p�X���쐬����B
			char* def_path = strjoinv("__", v);
			//�V���{����`���o�͂���B
			fprintf(fp, "//%s\n", err_path/*�R�����g�ɏo�͂���p�X�́A"__"��؂����"."��؂�̕������Ղ��Ǝv�����̂ŁAdef_path�ł͂Ȃ�err_path���g�����ɂ����B*/);
			if(x_str) { fprintf(fp, "#define X__%s\t%d\n", def_path, lround(localVar.x + x0)); }	//'+x0'��Y��Ȃ��悤��!!
			if(y_str) { fprintf(fp, "#define Y__%s\t%d\n", def_path, lround(localVar.y + y0)); }	//'+y0'��Y��Ȃ��悤��!!
			if(w_str) { fprintf(fp, "#define W__%s\t%d\n", def_path, lround(localVar.w)); }
			if(h_str) { fprintf(fp, "#define H__%s\t%d\n", def_path, lround(localVar.h)); }
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
		process_node(fp, fname, d, yaml_node_get_mapping(n), v2, localVar.x + x0, localVar.y + y0);	//����(!x_str)�Ȃ��(x==0)�Ȃ̂ŁAx_str�����������ɉ��Z���đ��v�ł��By_str��y�ɂ��Ă����l�ł��B
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
					g_dataset_set_data(fnGetVar_global, name, pd);	//���l���f�[�^�Z�b�g�ɓo�^����B
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
			process_node(fp, fname, d, m, v, 0.0, 0.0);
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
