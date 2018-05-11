import json
import re
import pprint
from datetime import datetime
import argparse


def convert(input_json_text):
    # Remove bad trailing commas causing malformed JSON
    # Apparently whatever tool spits out the input is not producing true JSON, only
    # technically valid javascript literals.
    # This could screw with string constants but the input so far does not seem to have any....
    cleaned = re.sub(r'\},(\s*)\}', r'}\1}', input_json_text)

    root = json.loads(cleaned)

    # Collect pairs of (date, commit_count)
    pairs = []

    for datestr, val in root.get('commits', '{}').items():
        # print(date,len(val))
        date = datetime.strptime(datestr, '%b %d, %Y')
        #print(date, len(val))
        pairs.append((date, len(val)))

    pairs.sort()

    # Produce output structure
    output_obj = {
        'postfix': 'Units',
        'color': 'green',
        'data': []
    }
    for date, cnt in pairs:
        output_obj['data'].append({
            'date': date.strftime('%Y-%m-%d'),
            'value': cnt
        })
    return output_obj


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('inputfile', help='Input file path')
    args = parser.parse_args()

    output_obj = convert(open(args.inputfile, 'r').read())
    print(json.dumps(output_obj, indent=2))

if __name__ == '__main__':
    main()
