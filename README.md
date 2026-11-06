# CLI 기반 이미지 처리 프로그램 구현

## 과제 목표
>CLI(Command Line Interface) 기반으로 동작하는 이미지 처리 도구를 완성합니다.

## 구현 항목

### 기본 요구사항
  - Grayscale 변환
  - 밝기 / 대비 조절
  - 이진화 (Thresholding)
  - 3x3 Convolution 필터
  - 히스토그램 분석
  - 이미지 크롭 / 리사이즈
  - 상하 / 좌우 반전

### 고급 요구사항
  - 필터 파이프라인 체인
  - 추상 클래스 기반 설계
  - 멀티쓰레드 병렬 처리
  - 로그 파일 출력

## 빌드

  Visual Studio 2022에서 `ImageProcessor.sln`을 열고 **Release / x64**로 빌드합니다. 실행 파일은 `.\x64\Release\ImageProcessor.exe`에 만들어집니다.

## 실행 명령어

솔루션 폴더(`ImageProcessor.sln`이 있는 곳)에서 PowerShell로 실행합니다.

```powershell
# Grayscale
.\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Output\1_astronaut_grayscale.bmp --filter grayscale

# Brightness (+50)
.\x64\Release\ImageProcessor.exe --input .\Resource\2_coffee.bmp --output .\Output\2_coffee_brightness.bmp --filter brightness:50

# Contrast (1.5배)
.\x64\Release\ImageProcessor.exe --input .\Resource\2_coffee.bmp --output .\Output\2_coffee_contrast.bmp --filter contrast:1.5

# Thresholding
.\x64\Release\ImageProcessor.exe --input .\Resource\4_text_page.bmp --output .\Output\4_text_page_threshold.bmp --filter threshold:128

# Blur
.\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Output\3_chelsea_cat_blur.bmp --filter blur

# Sharpen
.\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Output\3_chelsea_cat_sharpen.bmp --filter sharpen

# Histogram (콘솔 출력, 이미지는 그대로 저장)
.\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Output\1_astronaut_histogram.bmp --filter histogram

# Crop (x=100, y=50부터 200 x 150)
.\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Output\1_astronaut_crop.bmp --filter crop:100:50:200:150

# Resize (256 x 256)
.\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Output\1_astronaut_resize.bmp --filter resize:256:256

# Flip (좌우 / 상하)
.\x64\Release\ImageProcessor.exe --input .\Resource\5_checkerboard.bmp --output .\Output\5_checkerboard_flip_horizontal.bmp --filter flip:horizontal
.\x64\Release\ImageProcessor.exe --input .\Resource\5_checkerboard.bmp --output .\Output\5_checkerboard_flip_vertical.bmp --filter flip:vertical
```

### 파이프라인, 로그, 멀티쓰레드

```powershell
# 필터 파이프라인 체인
.\x64\Release\ImageProcessor.exe --input .\Resource\3_chelsea_cat.bmp --output .\Output\3_chelsea_cat_pipeline.bmp --pipeline "grayscale, blur, threshold:128"

# 로그 파일
.\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Output\1_astronaut_pipeline.bmp --pipeline "crop:100:50:300:300, resize:600:600, sharpen" --log .\Output\run.log

# 멀티쓰레드 (Blur/Sharpen, 쓰레드 4개)
.\x64\Release\ImageProcessor.exe --input .\Resource\1_astronaut.bmp --output .\Output\1_astronaut_blur_mt.bmp --filter blur --threads 4 --log .\Output\run.log
```

### 오류 예시

```powershell
# 알 수 없는 필터 → 종료 코드 3
.\x64\Release\ImageProcessor.exe -i .\Resource\1_astronaut.bmp -o .\Output\error.bmp -f nope

# 범위 밖의 값 → 종료 코드 3
.\x64\Release\ImageProcessor.exe -i .\Resource\1_astronaut.bmp -o .\Output\error.bmp -f threshold:300

# --filter와 --pipeline을 함께 지정 → 종료 코드 4
.\x64\Release\ImageProcessor.exe -i .\Resource\1_astronaut.bmp -o .\Output\error.bmp -f blur -p "grayscale"
```

## 구조

```text
main.cpp               인자 파싱 → 이미지 로드 → 필터 생성(FilterFactory) → 적용 → 저장
CommandLineParser      --input, --output, --filter, --pipeline, --log, --threads 해석
FilterFactory          필터 이름 문자열 → FilterBase 객체 (createFilter, createPipeline)
FilterBase             모든 필터의 추상 부모 클래스
*Filter                각 알고리즘 구현
PixelUtils.h           밝기 계산 공용 함수 (Grayscale, Threshold, Histogram에서 사용)
Logger                 로그 파일 기록
```


