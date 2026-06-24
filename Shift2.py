from itertools import pairwise

class ShiftAprox:
  __slots__ = "text", "pattern", "masks", "table", "match"

  def __init__(self, text: str, pattern: str, k: int = 1):
    self.text, self.pattern = text, pattern
    self.calculate_masks(); self.shift_aprox(k)
    print(f"Tabela das máscaras: {self.table}")
    print(f"Matchs encontrados: {self.match}")

  def calculate_masks(self):
    self.masks = dict.fromkeys(self.pattern, 0)
    for i, char in enumerate(self.pattern): self.masks[char] |= (1 << i)
    self.table = ", ".join(f"{m}: {v:b}" for m, v in self.masks.items())

  def shift_aprox(self, k: int):
    def execute_core():
      RK = [(1 << i) - 1 for i in range(k + 1)]
      C = 1 << (len(self.pattern) - 1)
      for i, char in enumerate(self.text):
        AT, mask = RK[:], self.masks.get(char, 0)
        RK[0] = ((AT[0] << 1) | 1) & mask
        if RK[0] & C: yield f"exato no índice {i}"
        for a, d in pairwise(range(k + 1)):
          RK[d] = ((AT[d] << 1) & mask) | (RK[a] << 1) | AT[a] | 1
          if (RK[d] & C) & ~(RK[a] & C): yield f"aprox. no índice {i}"
    self.match = ", ".join(execute_core())

ShiftAprox("os testes testam os alunos com um texto de capacidade", "teste")