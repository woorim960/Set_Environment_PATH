// 초기 설정에 필요한 헤더들 모음
#include "../../header/init_header.h"

// 환경 변수가 저장될 구조체 및 상수의 크기들을 정의한 헤더
#include "../../header/env_param_struct.h"

// delete_param.c에 필요한 함수들 모음
#include "../../header/func_delete.h"

// delete_param.c의 메인 함수
void delete_param(char file[]) {

    // 환경 변수를 담을 구조체 변수 선언
    ENV_PARAMETER env_param;

    // 변수 선언
    int fd, copy_fd, data_len;           // 파일디스크립터와 파일에 현재 저장된 데이터의 길이를 담을 변수
    char buffer[BUFFER_SIZE];   // 파일의 데이터가 저장될 변수
    int seq;                    // 선택한 시퀀스를 저장할 변수
    int cursor = 1;                 // 시퀀스의 이동 위치를 조정할 커서 변수

    // 파일 오픈
    if ( (fd = open(file, O_RDWR | O_CREAT, 0764)) == -1 ) {
        fprintf(stderr, "파일 삭제 오픈 에러 : 4\n");
        exit(4);
    }

    // 복사할 파일 오픈
    if ( (copy_fd = open("copy", O_RDWR | O_CREAT | O_TRUNC, 0764)) == -1 ) {
        fprintf(stderr, "파일 삭제 복사본 오픈 에러 : 4\n");
        exit(4);
    }

    // 삭제할 레코드가 있을 때 까지 반복
    while (1) {

        // 삭제할 시퀀스 선택
        puts("삭제할 시퀀스를 입력하세요(종료 : -1) :");
        printf(">> ");
        scanf("%d", &seq);

        // 종료
        if (seq == -1) {
            system("rm copy");
            close(fd);
            close(copy_fd);
            return;
        }

        // 삭제할 시퀀스로 이동
        lseek(fd, (seq -1) * sizeof(env_param), SEEK_SET);
        data_len = read(fd, &env_param, sizeof(env_param));

        if (data_len == 0) {
            // 삭제할 레코드가 없음
            puts("존재하지 않는 시퀀스입니다.");
        } else break;
    }

    // 삭제할 레코드 존재 여부 확인
    if (data_len == -1) {
        // 읽기 에러
        fprintf(stderr, "파일 수정 읽기 에러 : 3\n");
        exit(3);
    } 

    // 복사 함수(복사될 fd, 붙여넣을 fd, 구조체 변수, 삭제할 시퀀스, 커서 변수)
    copy(fd, copy_fd, env_param, seq, cursor);

    close(fd);
    // 파일의 기존 데이터를 모두 지운채로 오픈
    if ( (fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0764)) == -1 ) {
        fprintf(stderr, "파일 삭제 오픈 에러 : 4\n");
        exit(4);
    }

    // 재복사 함수(복사될 fd, 붙여넣을 fd, 구조체 변수, 커서 변수)
    recopy(copy_fd, fd, env_param, cursor);

    // 터미널 컨트롤
    system("rm copy"); // copy 삭제
    system("clear");   // 터미널 클리어

    // 출력
    puts("삭제되었습니다.");

    // 파일 디스크립터 close
    close(fd); 
    close(copy_fd);
    return;
}

// 복사 함수(복사될 fd, 붙여넣을 fd, 구조체 변수, 삭제할 시퀀스, 커서 변수)
int copy(int fd, int copy_fd, ENV_PARAMETER env_param, int seq, int cursor) {
    int data_len;

    // 시퀀스의 마지막 번호를 알아냄
    lseek(fd, -sizeof(env_param), SEEK_END);
    data_len = read(fd, &env_param, sizeof(env_param));

    // 복사할 데이터가 없으면 함수 종료
    if (is_empty(data_len)) return -1;
    int end_cursor = env_param.seq;

    // 모든 환경 변수 레코드를 복사하기 위함
    while(cursor <= end_cursor) {

        // 처음부터 시퀀스 레코드만 제외하고 copy에 복사
        if (cursor != seq) {
            lseek(fd, (cursor -1) * sizeof(env_param), SEEK_SET);
            data_len = read(fd, &env_param, sizeof(env_param));

            // 복사할 데이터가 없으면 함수 종료
            if (is_empty(data_len)) return -1;
            
            write(copy_fd, &env_param, sizeof(env_param));
            cursor += 1;
        } else {
            cursor += 1;
        }
    }

    return 1;
}

// 재복사 함수(복사될 fd, 붙여넣을 fd, 구조체 변수, 커서 변수)
int recopy(int fd, int copy_fd, ENV_PARAMETER env_param, int cursor) {
    int data_len;

    // 시퀀스의 마지막 번호를 알아냄
    lseek(fd, -sizeof(env_param), SEEK_END);
    data_len = read(fd, &env_param, sizeof(env_param));

    // 복사할 데이터가 없으면 함수 종료
    if (is_empty(data_len)) return -1;
    int end_cursor = env_param.seq;

    // 모든 환경 변수 레코드를 복사하기 위함
    while(cursor <= end_cursor) {
        
        lseek(fd, (cursor -1) * sizeof(env_param), SEEK_SET);
        data_len = read(fd, &env_param, sizeof(env_param));

        // 복사할 데이터가 없으면 함수 종료
        if (is_empty(data_len)) return -1;
        
        env_param.seq = cursor;
        write(copy_fd, &env_param, sizeof(env_param));
        cursor += 1;
    }

    return 1;
}

// 읽을 데이터가 있는지 확인하기 위한 함수 정의
int is_empty(int data_len) {
    if (data_len == 0) {
        return 1;
    } else if (data_len == -1) {
        puts("읽기 에러 : 1");
        exit(1);
    }

    return 0;
}