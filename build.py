"""Build script for epseon_backend package."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path


class Builder:
    """Class responsible for building epseon_backend binaries."""

    def __init__(self) -> None:
        """Initialize builder object."""
        self.repo_path = Path(__file__).parent
        self.prepare_submodules()

    def prepare_submodules(self) -> None:
        """Prepare dependency submodules."""
        self.git("submodule", "init")
        self.git(
            "-C",
            f"{self.repo_path.as_posix()}/external/googletest",
            "fetch",
            "--tags",
        )
        self.git(
            "-C",
            f"{self.repo_path.as_posix()}/external/googletest",
            "checkout",
            "release-1.12.1",
        )

    def git(self, *arg: str) -> None:
        """Run git command."""
        try:
            subprocess.run(
                args=[
                    "git",
                    *arg,
                ],
                cwd=self.repo_path.as_posix(),
                capture_output=True,
                check=True,
            )
        except subprocess.CalledProcessError as e:
            sys.stdout.write(e.stdout.decode("utf-8"))
            sys.stderr.write(e.stderr.decode("utf-8"))
            raise

    def build(self) -> None:
        """Build extension module."""
        self.cmake("-S", ".", "-B", "build")
        self.cmake("--build", "build", "--target", "epseon_cpu")
        self.cmake("--build", "build", "--target", "epseon_gpu")

    def cmake(self, *arg: str) -> None:
        """Run cmake command. If fails, raises CalledProcessError."""
        try:
            subprocess.run(
                executable=sys.executable,
                args=[
                    sys.executable,
                    "-c",
                    "import cmake;cmake.cmake()",
                    *arg,
                ],
                cwd=self.repo_path.as_posix(),
                capture_output=True,
                check=True,
            )
        except subprocess.CalledProcessError as e:
            sys.stdout.write(e.stdout.decode("utf-8"))
            sys.stderr.write(e.stderr.decode("utf-8"))
            raise


Builder().build()