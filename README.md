# 🚀 SIMD-Accelerated 3D Transformer

본 프로젝트는 **x86 SSE(Streaming SIMD Extensions)** 명령어를 활용하여 3차원 공간상의 점들을 고속으로 연산하는 **SIMD 가속 공간 변환 라이브러리**입니다. 3D 그래픽스 파이프라인의 핵심인 정점 변환(Vertex Transformation) 과정을 어셈블리 수준에서 최적화하여 연산 효율을 극대화했습니다.

## ✨ Key Features

* **SIMD Optimization**: x86 SSE의 `dpps` (Dot Product), `movaps`, `addps` 명령어를 사용하여 행렬 곱셈 및 벡터 변환 연산을 하드웨어 레벨에서 가속화했습니다.
* **Memory Alignment**: SIMD 명령어의 성능을 최대로 끌어내기 위해 모든 데이터 구조체(`vec4_t`, `mat4_t`)를 16바이트 경계로 정렬(`__declspec(align(16))`)하여 설계했습니다.
* **Efficient Transformation Pipeline**: Scale, Rotation(X, Y, Z), Translation의 개별 행렬 생성 및 결합 기능을 제공고 SIMD 내적 연산에 최적화된 행렬 전치(Transpose) 로직을 인라인 어셈블리로 구현했습니다.
* **Left-Handed Coordinate System**: 3D 그래픽스에서 널리 사용되는 왼손 좌표계 기반의 변환 시스템을 지원합니다.

## 🛠 Tech Stack

* **Language**: C
* **Instruction Set**: x86 SSE (Inline Assembly)
* **Mathematics**: Linear Algebra (Matrix & Vector Multiplication)

## 🏗 System Architecture

이 프로그램은 로컬 좌표계의 점들을 월드 좌표계로 변환하기 위해 다음과 같은 수학적 파이프라인을 따릅니다.

1. **Matrix Generation**: 개별 변환 행렬(S, R, T)을 생성합니다.
2. **Transposition**: SIMD 내적 연산 구조에 맞춰 행렬의 행과 열을 교환합니다.
3. **Concatenation**: 생성된 모든 행렬을 하나의 최종 변환 행렬로 결합합니다.
4. **Vertex Transformation**: 결합된 행렬을 각 정점에 적용하여 최종 월드 좌표를 산출합니다.

## 💻 Core Logic: SIMD Dot Product

`dpps` 명령어를 활용하여 4개 요소의 내적을 한 번에 계산함으로써, 기존 스칼라 연산 대비 월등한 처리 속도를 확보했습니다.

```c
/* transformer.c */
void transform(vec4_t* dst, const vec4_t* src, const mat4_t* mat_tr) {
    __asm {
        mov eax, mat_tr
        mov ebx, src
        // ... (생략)
        
        // 벡터와 행렬 각 행의 내적 연산을 단일 명령어로 수행
        dpps xmm0, xmm4, 11110001b 
        
        // ...
        mov eax, dst
        movaps [eax], xmm0
    }
}

```

## 📈 Performance Optimization

이 라이브러리는 성능 최적화를 위해 다음과 같은 전략을 사용합니다.

* **Matrix Concatenation**: 모든 변환 행렬을 하나로 합친 뒤 점들에 적용합니다. 이는 점의 개수가 개일 때 연산 복잡도를 각 변환마다 점을 계산하는 방식보다 획기적으로 줄여줍니다.
* **Batch Processing**: `run` 함수를 통해 대량의 정점 데이터를 루프 내에서 일괄 처리하여 캐시 효율성을 높였습니다.

## 🚀 How to Run

1. **환경**: x86 아키텍처 및 MSVC 인라인 어셈블리를 지원하는 컴파일러가 필요합니다.
2. **입력**: 표준 입력으로 각 점의 좌표()를 입력합니다 (최대 512개).
3. **출력**: 변환이 완료된 좌표값이 CSV 형식으로 출력됩니다.
