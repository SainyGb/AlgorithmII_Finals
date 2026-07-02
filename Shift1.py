class ShiftLeft:
  __slots__ = "text", "pattern", "masks", "table", "match"

  def __init__(self, text: str, pattern: str):
    self.text, self.pattern = text, pattern
    self.calculate_masks(); self.shift_left()
    print(f"Tabela das máscaras: {self.table}")
    print(f"Matchs nos índices: {self.match}")

  def calculate_masks(self):
    self.masks = dict.fromkeys(self.pattern, 0)
    for i, char in enumerate(self.pattern): self.masks[char] |= (1 << i)
    self.table = ", ".join(f"{m}: {v:b}" for m, v in self.masks.items())

  def shift_left(self):
    R, C = 0, 1 << (len(self.pattern) - 1)
    self.match = ", ".join(str(i) for i, char in enumerate(self.text)
      if (R := self.masks.get(char, 0) & ((R << 1) | 1)) & C)

ShiftLeft("os testes testam os alunos com um teste de capacidade", "teste")