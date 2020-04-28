// 헤더파일
#include <stdio.h>
#include <stdlib.h> // system() 호출을 위한 헤더

// 함수의 선언들을 하나의 헤더 파일로 정의하였음.
#include "header/func_define.h"

//  메인
int main(int argc, char *argv[]) {

    // 변수 선언
    int selector; // 원하는 작업으로 분기하기 위해 사용될 선택자 변수
    char file[20]; // 파일 이름을 저장할 변수
    int init_value = 1; // 처음에 조회될 레코드의 시퀀스를 설정하기 위한 변수

    // 사용할 파일 이름을 입력받는 함수 실행
    file_input(file);

    // 터미널을 clear하기 위함.
    system("clear");

    // 초기 출력문 출력
    print_init(file, init_value);

    while(1) {
        // 선택자 입력 받기
        scanf("%d", &selector);
        getchar(); // 버퍼 비우기

        // 반복 때마다 터미널을 clear하기 위함.
        system("clear");

        // 선택자에 따라 원하는 작업으로 분기
        switch(selector) {
            case 1:
                // 환경 변수 입력
                write_param(file);
                print_init(file, init_value);
                break;
            case 2:
                // 환경 변수 수정
                print_init(file, init_value);
                edit_param(file);
                system("clear");
                print_init(file, init_value);
                break;
            case 4:
                // 환경 변수 삭제
                print_init(file, init_value);
                delete_param(file);
                system("clear");
                print_init(file, init_value);
                break;
            case 7:
                // 환경 변수 조회 5칸 앞으로 이동
                init_value -= 5;
                if (init_value < 1) {
                    // 초기 값이 1보다 작을 경우 1로 초기화
                    init_value = 1;
                }
                init_value = print_init(file, init_value);
                break;
            case 8:
                // 환경 변수 조회 5칸 뒤로 이동
                init_value += 5;
                init_value = print_init(file, init_value);
                break;
            case 9:
                // 메인 함수 종료
                exit_main(file);
                break;
        }
    }

    return 0;
}



/*
개발자 정의 함수
*/


// 파일 입력을 받기 위한 함수 정의
void file_input(char file[]) {
    // 터미널을 clear하기 위함.
    system("clear");
    puts("");
    puts("                ***************************************");
    puts("                 사용하실 파일을 공백없이 입력해주세요");
    puts("                ***************************************");
    puts("\n해당 파일은 본 프로그램의 PATH와 같은 PATH에 존재해야 합니다.");
    puts("주의 : 파일이 없을 경우 생성됩니다.\n");
    printf(">> ");
    fscanf(stdin, "%s", file);
    getchar();
}

// 초기 출력문 출력함수 정의
int print_init(char file[], int init_value) {
    puts("");
    puts("    ================ 환경 변수 설정 화면 ================\n");
    puts("    Seq    Parameter            Parameter value");
    puts("    -----  -----------------    -----------------------");

    // 환경 변수 조회 함수 실행
    init_value = read_param(file, init_value);

    printf("\n\n    FILE_NAME : %s\n", file);
    puts("    =====================================================");
    puts("    1:입력   2:수정   4:삭제   7:앞으로   8:뒤로   9:종료");
    puts("    원하는 작업을 선택하세요 :\n");
    printf(">> ");

    return init_value;
}