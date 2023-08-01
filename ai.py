import os

import openai
from dotenv import load_dotenv, find_dotenv

_ = load_dotenv(find_dotenv())
openai.api_key = os.getenv('OPENAI_API_KEY')
print(os.getenv('OPENAI_API_KEY'))


def get_completion(prompt, model='gpt-3.5-turbo'):
    messages = [{"role": "user", "content": prompt}]
    response = openai.ChatCompletion.create(
        model=model,
        messages=messages,
        temperature=0,
    )
    return response.choices[0].message["content"]


spec = """
OVERVIEW
- Part of a breutiful family of mid-century inspired office
- Several options of shell color and base finishes.

CONSTRUCTION
- 5-wheel plastic coated aluminum base.

DIMENSIONS
- WIDTH 53 CM | 20.87"
- DEPTH 53 CM | 20.87"

OPTIONS
- Soft or hrd-floor caster options.

MATERIALS
SHELL BASE GLIDER

COUNTRY ORIGIN
- Italy
"""
prompt = f"""
Your task is to help a marketing team create a description for a retail website of a product based on a technical 
fact sheet.

Write a product description based on the information provided in the technical specifications
delimited by triple backticks.

Use at most 50 words.

Technical specifications:```{spec}```
"""
result = get_completion(prompt)
print(result)
