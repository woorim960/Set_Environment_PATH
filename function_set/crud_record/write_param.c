// 초기 설정에 필요한 헤더들 모음
#include "../../header/init_header.h"

// 환경 변수가 저장될 구조체 및 상수의 크기들을 정의한 헤더
#include "../../header/env_param_struct.h"

// write_param.c의 메인 함수
void write_param(char file[]) {

    // 환경 변수를 담을 구조체 변수 선언
    ENV_PARAMETER env_param;

    // 변수 선언
    int fd, data_len;           // 파일디스크립터와 파일에 현재 저장된 데이터의 길이를 담을 변수
    char buffer[BUFFER_SIZE];   // 파일의 데이터가 저장될 변수

    // 파일 오픈
    if ( (fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0764)) == -1 ) {
        fprintf(stderr, "파일 입력 오픈 에러 : 2\n");
        exit(2);
    }

    // 추가될 데이터에게 맞는 시퀀스를 부여하기 위한 설정 단계
    if ( (data_len = read(fd, buffer, BUFFER_SIZE)) == 0) {
        // 데이터가 없으면 시퀀스는 1
        env_param.seq = 1;
    } else if (data_len == -1) {
        // 읽기 에러
        fprintf(stderr, "파일 입력 읽기 에러 : 2\n");
        exit(2);
    } else {
        // 데이터가 있으면 마지막 시퀀스에서 +1한 값을 저장한다.
        lseek(fd, -sizeof(env_param), SEEK_END);
        read(fd, &env_param, sizeof(env_param));
        env_param.seq = env_param.seq +1;
    }

    // 파라미터 키 입력
    puts("\nParameter :");
    printf(">>  ");
    fscanf(stdin, "%s", env_param.key_param);
    getchar();

    // 파라미터 값 입력
    puts("\nParameter Value :");
    printf(">>  ");
    fgets(env_param.val_param, VALUE_SIZE, stdin);

    // 저장 전 파일의 끝으로 이동
    if (lseek(fd, 0, SEEK_END) == -1) {
        puts("파일 입력 이동 에러 : 2\n");
        exit(2);
    }

    // 파일에 저장
    if ( write(fd, &env_param, sizeof(env_param)) == -1 ) {
        fprintf(stderr, "파일 쓰기 에러 : 2\n");
        exit(2);
    }

    // 터미널 클리어
    system("clear");

    // 파일 디스크립터 close
    close(fd);
    return;
}