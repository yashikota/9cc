#include "9cc.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    /// DEBUG
    // fprintf(stderr, "Input: %s\n", argv[1]);
    /// DEBUG

    // グローバル変数の extern 宣言
    extern Node *code[100];
    extern char *user_input;

    // トークナイズしてパースする
    user_input = argv[1];
    tokenize();
    program();

    /// DEBUG
    // fprintf(stderr, "Parsed program:\n");
    // for (int i = 0; code[i]; i++) {
    //     fprintf(stderr, "  Node %d: kind=%d\n", i, code[i]->kind);
    // }
    /// DEBUG

    // アセンブリの前半部分を出力
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    // プロローグ
    // 変数26個分の領域を確保する
    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, 208\n"); // 一つの変数は8バイトなので26個分で208バイト

    // 先頭の式から順にコード生成
    for (int i = 0; code[i]; i++) {
        gen(code[i]);

        // 式の評価結果としてスタックに一つの値が残っている
        // はずなので、スタックが溢れないようにポップしておく
        printf("    pop rax\n");
    }

    // エピローグ
    // 最後の式の結果がRAXに残っているのでそれが返り値になる
    printf("    mov rsp, rbp\n");
    printf("    pop rbp\n");
    printf("    ret\n");

    /// DEBUG
    // fprintf(stderr, "Generated assembly:\n");
    // for (int i = 0; code[i]; i++) {
    //     fprintf(stderr, "  Node %d: kind=%d\n", i, code[i]->kind);
    // }
    /// DEBUG

    return 0;
}
