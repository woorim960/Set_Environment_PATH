// 초기 설정에 필요한 헤더들 모음
#include "../../header/init_header.h"

// 환경 변수가 저장될 구조체 및 상수의 크기들을 정의한 헤더
#include "../../header/env_param_struct.h"

// read_param.c의 메인 함수
int read_param(char file[], int init_value) {

    // 환경 변수를 담을 구조체 변수 선언
    ENV_PARAMETER env_param;

    // 변수 선언
    int fd, data_len;     // 파일디스크립터와 파일에 현재 저장된 데이터의 길이를 담을 변수
    char buffer[BUFFER_SIZE];   // 파일의 데이터가 저장될 변수

    // 파일 오픈
    if ( (fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0764)) == -1 ) {
        fprintf(stderr, "파일 조회 오픈 에러 : 1\n");
        exit(1);
    }

    // 파일 읽기
    if ( (data_len = read(fd, buffer, BUFFER_SIZE)) == -1) {
        // 읽기 에러
        fprintf(stderr, "파일 조회 읽기 에러 : 1\n");
        exit(1);
    }

    // 레코드를 5개 조회하기 위해 5번 순회
    for (int i=0; i<5; i++) {

        // 레코드 읽기
        lseek(fd, (init_value -1)*sizeof(env_param), SEEK_SET);     // init_value를 1씩 증가시키면서 레코드 이동
        data_len = read(fd, &env_param, sizeof(env_param));         // 해당 위치에서 레코드 읽기

        // 조회할 레코드가 있을 경우에만 출력하기 위한 단계
        if (data_len > 0) {
            // 조회할 레코드가 있을 경우에만 출력
            printf("    %-6d %-20s %s", env_param.seq, env_param.key_param, env_param.val_param);
        } else if (data_len == 0 && init_value%5 == 1) {    // 데이터가 없고 init_value의 시작 위치가 1, 6, 11, 16 .. 일 경우에 실행
            // 뒤로 이동할 경우에 데이터가 없으면 다시 앞의 레코드를 보여줄 수 있도록 하기 위함
            init_value -= 5;

            // 레코드 읽기
            lseek(fd, (init_value -1)*sizeof(env_param), SEEK_SET);     // init_value의 -5된 위치로 레코드 이동
            data_len = read(fd, &env_param, sizeof(env_param));         // 해당 위치에서 레코드 읽기

            if (data_len > 0) {
                // 조회할 레코드가 있을 경우에만 출력
                printf("    %-6d %-20s %s", env_param.seq, env_param.key_param, env_param.val_param);
            }
        }

        // init_value 1 증가
        init_value += 1;
    }

    // 할당된 메모리 반환
    close(fd);

    // 위에서 반복문으로 인해 init_value가 5 증가했으므로 다시 5를 감소한 뒤에 반환
    init_value -= 5;
    return init_value;
}