from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import os
import json
import time
import requests

URL = "https://project-chameleon.vercel.app/"
OUTPUT_FOLDER = "output_stories"
JSON_FILE = os.path.join(OUTPUT_FOLDER, "stories_data.json")

# Create base folder
if not os.path.exists(OUTPUT_FOLDER):
    os.makedirs(OUTPUT_FOLDER)

options = webdriver.ChromeOptions()
options.add_argument("--start-maximized")
driver = webdriver.Chrome(options=options)
wait = WebDriverWait(driver, 15)

stories_data = []

def download_image(url, save_path):
    try:
        r = requests.get(url, timeout=10)
        if r.status_code == 200:
            with open(save_path, "wb") as f:
                f.write(r.content)
    except:
        print(f"Failed to download image: {url}")

try:
    driver.get(URL)
    wait.until(EC.presence_of_element_located((By.TAG_NAME, "body")))

    # Locate story section
    story_section = wait.until(
        EC.presence_of_element_located((By.XPATH, "//div[@data-section='stories']"))
    )
    story_users = story_section.find_elements(By.XPATH, ".//div[@data-story-id]")

    for story_user in story_users:

        try:
            username_elem = story_user.find_element(By.XPATH, ".//p")
            username = username_elem.text.strip()

            print(f"\n========== SCRAPING {username} ==========\n")

            # Create user folder
            user_folder = os.path.join(OUTPUT_FOLDER, username)
            if not os.path.exists(user_folder):
                os.makedirs(user_folder)

            # Scroll + click the user's story
            driver.execute_script("arguments[0].scrollIntoView({block: 'center'});", story_user)
            time.sleep(0.3)
            driver.execute_script("arguments[0].click();", story_user)
            time.sleep(1)

            user_stories = []
            previous_imgs = set()
            story_index = 1

            while True:
                try:
                    # Wait for story image
                    story_img = wait.until(
                        EC.presence_of_element_located(
                            (By.XPATH, "//div[contains(@class,'aspect-[9/16]')]//img")
                        )
                    )

                    img_url = story_img.get_attribute("src")
                    caption = story_img.get_attribute("alt")

                    # ❌ STOP if it switched to next user's story
                    if caption and not caption.startswith(f"{username}'s"):
                        print(f"Reached next user's story ({caption}) — stopping.")
                        break

                    if img_url not in previous_imgs:
                        local_path = os.path.join(user_folder, f"story_{story_index}.jpg")
                        download_image(img_url, local_path)

                        user_stories.append({
                            "img_url": img_url,
                            "caption": caption,
                            "local_path": local_path.replace("\\", "/")
                        })

                        previous_imgs.add(img_url)
                        story_index += 1

                    # Click next story
                    next_btn_area = driver.find_element(
                        By.XPATH,
                        "//div[contains(@class,'aspect-[9/16]')]//div[@class='flex-1 cursor-pointer'][2]"
                    )
                    driver.execute_script("arguments[0].click();", next_btn_area)
                    time.sleep(0.5)

                except Exception as e:
                    print(f"No more stories or error occurred: {e}")
                    break

            # Append user story data
            stories_data.append({
                "username": username,
                "stories": user_stories
            })

            # Close modal
            try:
                close_btn = wait.until(
                    EC.element_to_be_clickable((
                        By.XPATH,
                        "//div[@role='dialog']//button[@aria-label='Close' or @type='button']"
                    ))
                )
                driver.execute_script("arguments[0].click();", close_btn)
            except:
                print("Modal already closed or close button not found.")

            time.sleep(0.5)

        except Exception as e:
            print(f"Error with user {username}: {e}")

    # Save JSON
    with open(JSON_FILE, "w", encoding="utf-8") as f:
        json.dump(stories_data, f, ensure_ascii=False, indent=4)

    print("\n======= SCRAPING COMPLETED =======")
    print(f"Stories saved to folder: {OUTPUT_FOLDER}")
    print(f"JSON: {JSON_FILE}\n")

finally:
    input("Press Enter to close browser...")
    driver.quit()
