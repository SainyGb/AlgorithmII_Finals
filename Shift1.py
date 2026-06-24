class ShiftAnd:
  __slots__ = "T", "P", "L", "masks", "match", "rmasks"

  def __init__(self, texto: str, padrão: str):
    self.T, self.P, self.L = texto, padrão, len(padrão)
    self.calculate(); self.execute()
    print(f"Tabela das máscaras: {self.rmasks}")
    print(f"Matchs encontrados: {self.match}")

  def calculate(self):
    self.masks = {char: 0 for char in set(self.P)}
    for i, char in enumerate(reversed(self.P)): self.masks[char] |= 1 << i
    self.rmasks = ", ".join(f"{m}: {v:b}" for m, v in self.masks.items())

  def execute(self):
    def execute_core():
      R, OR = 0, 1 << (self.L - 1)
      for i, char in enumerate(self.T):
        R = ((R >> 1) | OR) & self.masks.get(char, 0)
        if R & 1: yield f"no índice {i}"
    self.match = ", ".join(execute_core())

ShiftAnd("os testes testam os alunos com um teste de capacidade", "teste")