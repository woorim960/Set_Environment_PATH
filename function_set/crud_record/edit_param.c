// 초기 설정에 필요한 헤더들 모음
#include "../../header/init_header.h"

// 환경 변수가 저장될 구조체 및 상수의 크기들을 정의한 헤더
#include "../../header/env_param_struct.h"

// edit_param.c의 메인 함수
void edit_param(char file[]) {

    // 환경 변수를 담을 구조체 변수 선언
    ENV_PARAMETER env_param;

    // 변수 선언
    int fd, data_len = 0;           // 파일디스크립터와 파일에 현재 저장된 데이터의 길이를 담을 변수
    char buffer[BUFFER_SIZE];   // 파일의 데이터가 저장될 변수
    int seq;               // 선택한 시퀀스를 저장할 변수

    // 파일 오픈
    if ( (fd = open(file, O_RDWR | O_CREAT, 0764)) == -1 ) {
        fprintf(stderr, "파일 수정 오픈 에러 : 3\n");
        exit(3);
    }

    // 정상 입력할 때 까지 반복
    while (1) {

        // 수정할 시퀀스 선택
        puts("수정할 시퀀스를 입력하세요(종료 : -1) :");
        printf(">> ");
        scanf("%d", &seq);

        // 종료
        if (seq == -1) return;

        // 수정할 시퀀스로 이동
        lseek(fd, (seq -1) * sizeof(env_param), SEEK_SET);
        data_len = read(fd, &env_param, sizeof(env_param));

        if (data_len == 0) {
            // 수정할 레코드가 없음
            puts("존재하지 않는 시퀀스입니다.");
        } else break;
    }

    // 수정할 레코드 존재 여부 확인
    if (data_len == -1) {
        // 읽기 에러
        fprintf(stderr, "파일 수정 읽기 에러 : 3\n");
        exit(3);
    }

    system("clear");

    // 수정할 레코드 출력
    puts("\n    ================ 수정할 환경 변수 ================\n");
    puts("    Seq    Parameter            Parameter value");
    puts("    -----  -----------------    -----------------------");
    printf("    %-6d %-20s %s\n", env_param.seq, env_param.key_param, env_param.val_param);

    // 시퀀스는 그대로 저장
    env_param.seq = seq;

    // 파라미터 키 입력
    puts("Parameter :");
    printf(">>  ");
    fscanf(stdin, "%s", env_param.key_param);
    getchar();

    // 파라미터 값 입력
    puts("Parameter Value :");
    printf(">>  ");
    fgets(env_param.val_param, VALUE_SIZE, stdin);

    // 수정할 시퀀스로 이동
    lseek(fd, (seq -1) * sizeof(env_param), SEEK_SET);

    // 파일 업데이트(쓰기와 동일)
    if ( write(fd, &env_param, sizeof(env_param)) == -1 ) {
        fprintf(stderr, "파일 수정 쓰기 에러 : 3\n");
        exit(3);
    }

    // 터미널 클리어
    system("clear");

    // 파일 디스크립터 close
    close(fd);
    return;
}