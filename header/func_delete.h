// func_delete.h

// 복사 함수(복사될 fd, 붙여넣을 fd, 구조체 변수, 삭제할 시퀀스, 커서 변수) 선언
int copy(int fd, int copy_fd, ENV_PARAMETER env_param, int seq, int cursor);
// 재복사 함수(복사될 fd, 붙여넣을 fd, 구조체 변수, 커서 변수) 선언
int recopy(int fd, int copy_fd, ENV_PARAMETER env_param, int cursor);
// 읽을 데이터가 있는지 확인하기 위한 함수 선언
int is_empty(int data_len);