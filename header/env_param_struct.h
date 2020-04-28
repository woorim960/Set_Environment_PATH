#define INPUT_SIZE 64       // 입력 데이터 크기
#define BUFFER_SIZE 128     // 파일 데이터 읽을 크기

#define START_SEQUENCE 1
#define KEY_SIZE 32
#define VALUE_SIZE 32

typedef struct _env_param {

    int seq;
    char key_param[KEY_SIZE];
    char val_param[VALUE_SIZE];

} ENV_PARAMETER;