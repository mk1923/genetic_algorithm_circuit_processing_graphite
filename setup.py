from setuptools import setup, find_packages

setup(
    name='graph-vector-project',
    version='0.1',
    packages=find_packages(),
    install_requires=[
        'graphviz==0.20.1',
        'Pillow==9.4.0'
    ],
    entry_points={
        'console_scripts': [
            'run-script=post_process.visualisation:main',
        ],
    },
    include_package_data=True,
    package_data={
        '': ['./post_process/vector_data.txt'],
    },
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ],
    python_requires='>=3.6',
)

