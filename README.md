# huffman

- 무손실 압축에 사용되는 Entropy Coding의 일종
- 심볼이 나올 확률에 따라 심볼의 길이를 다르게 하는 Variable Length Coding
- 나올 확률이 높은 심볼 : 짧은 길이의 Code를 가짐
- 나올 확률이 낮은 심볼 : 긴 길이의 Code를 가짐

Encoding

1. 히스토그램 생성 및 Linked List 구성

2. Linked List 정렬

3. 트리 구성
- 부모 node 생성 (확률 = 하위 2개 결합)
- Linked List 하위 2개 Node를 부모와 연결
- 생성한 부모 Node를 Linked List에 연결
- 자식 Node를 리스트에서 제외
- Linked List 정렬
- 이 과정 반복
- 최종 생성된 트리를 재귀호출을 통해서 각 픽셀값들에 코드 부여


Decoding

mapping된 table을 통해서 1bit씩 값을 확인해 나가며 영상을 복구한다.
