"""Bounded CLI parser regression; temporary files only, no external inputs."""
import pathlib
import subprocess
import sys
import tempfile
with tempfile.TemporaryDirectory() as directory:
    puzzle = pathlib.Path(directory) / "puzzle.txt"
    puzzle.write_text("." * 81)
    result = subprocess.run([sys.argv[1], str(puzzle)], capture_output=True, timeout=5)
    assert result.returncode == 0, result.stdout
    puzzle.write_text("." * (64 * 1024 + 1))
    result = subprocess.run([sys.argv[1], str(puzzle)], capture_output=True, timeout=5)
    assert result.returncode != 0, "Oversized puzzle was accepted"
    result = subprocess.run([sys.argv[1], str(puzzle) + ".missing"], capture_output=True, timeout=5)
    assert result.returncode != 0, "Missing puzzle reported success"
print("Puzzle input security checks passed")
