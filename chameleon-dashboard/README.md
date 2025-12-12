# Chameleon DSA Dashboard

A lightweight, real-time data processing dashboard built with vanilla HTML, CSS, and JavaScript. Connects directly to Supabase for live data from Instagram, WhatsApp, and Gaming platforms.

## 🚀 Quick Start

1. **No installation needed!** Just open `index.html` in your browser
2. The dashboard automatically connects to Supabase
3. Real-time data loads from your database

## 📁 Files

- **index.html** - Complete HTML structure with all pages
- **style.css** - Beautiful dark theme with gradients and responsive design
- **script.js** - Vanilla JavaScript with Supabase integration
- **README.md** - This file

## 🎯 Features

### Home Page
- Quick stats overview (total profiles, events, devices)
- Navigation cards to other sections

### Data Processing Menu
**4 Sorting Tabs:**

1. **Huffman Encoding** 🗜️
   - Data compression analysis
   - Frequency tables
   - Compression ratio calculation
   - Time Complexity: O(n log n)

2. **Instagram Sorting** 📸
   - Sort profiles by: followers, following, posts, user_id, created_at
   - Display: username, followers, following, posts, bio
   - Real-time data from Supabase

3. **WhatsApp Sorting** 💬
   - Sort events by: timestamp, device count, created_at
   - Filter by presence status
   - Display: phone number, presence, devices, IP, event time

4. **Gaming Sorting** 🎮
   - Search devices by ID
   - Sort by: device_id, language, timezone, created_at
   - Display: device ID, language, timezone, GPU, games

### Analytics Page
- **🏆 Ranking Techniques** - Priority Queue algorithms
- **🔗 Identity Unification** - Union-Find DSU
- **📝 Bio Analysis** - Inverted Index + TF-IDF
- **🔍 Duplicate Detection** - Bloom Filter + MinHash

### About Page
- Project purpose and overview
- Core technologies (Frontend, Database, Backend)
- 10 Key algorithms with icons
- 4 Real-world use cases

## 🔌 Supabase Connection

The dashboard connects to your Supabase project automatically:

```javascript
// Configured in script.js
const SUPABASE_URL = 'https://qdsbdmikobhynjmfzweb.supabase.co';
const SUPABASE_ANON_KEY = 'your-anon-key-here';
```

**Tables Connected:**
- `instagram_profiles` - Profiles with followers, following, posts
- `instagram_stories` - Story data with captions and images
- `whatsapp_events` - Events with presence and device count
- `game_devices` - Gaming devices with specs and games list
- `users` - User linking table

## 💻 Technologies

- **Frontend:** Vanilla HTML5, CSS3, JavaScript (ES6+)
- **Database:** Supabase (PostgreSQL)
- **SDK:** Supabase JavaScript Client
- **Styling:** Custom CSS with gradients and animations
- **No Dependencies:** Except Supabase JS SDK (loaded via CDN)

## 🧠 DSA Algorithms Implemented

1. **Sorting** - O(n log n) for profile/event ranking
2. **Linear/Binary Search** - O(n) device ID search
3. **Filtering** - O(n) presence status filtering
4. **Frequency Analysis** - Huffman encoding preparation
5. **Union-Find** - Identity unification concept
6. **Inverted Index** - Bio term analysis
7. **Bloom Filter** - Duplicate detection concept
8. **Priority Queue** - Ranking by followers
9. **Trie** - Prefix-based search structure
10. **MinHash** - Similarity estimation

## 📊 Data Flow

```
┌─────────────────┐
│   index.html    │
│   (Browser)     │
└────────┬────────┘
         │
    ┌────▼────┐
    │  CSS    │  (Styling)
    └────┬────┘
         │
    ┌────▼────────┐
    │  script.js   │  (Logic + Data)
    └────┬────────┘
         │
    ┌────▼──────────────┐
    │  Supabase JS SDK   │  (CDN)
    └────┬──────────────┘
         │
    ┌────▼──────────────────┐
    │  Supabase Backend      │
    │  (PostgreSQL Database) │
    └───────────────────────┘
```

## 🎨 UI/UX Features

- **Dark Theme** - Easy on the eyes, professional look
- **Gradient Cards** - Color-coded sections (Pink/Green/Blue)
- **Responsive Design** - Works on desktop, tablet, mobile
- **Smooth Animations** - Page transitions and hover effects
- **Live Indicator** - Shows connection status
- **Real-time Data** - Auto-loads from Supabase

## 📈 Performance Metrics

- **Page Size:** < 100KB total (no build step needed)
- **Load Time:** < 1 second
- **Database Queries:** O(1) for random access via Supabase
- **Sort Operations:** O(n log n) for up to 1000 records
- **Search:** O(n) for device ID linear search

## 🔧 Configuration

To use with your own Supabase project:

1. Update credentials in `script.js`:
```javascript
const SUPABASE_URL = 'your-project-url';
const SUPABASE_ANON_KEY = 'your-anon-key';
```

2. Ensure your Supabase tables have matching column names:
   - `instagram_profiles`: id, user_id, username, followers_count, following_count, posts_count, bio
   - `whatsapp_events`: id, user_id, phone_number, event_time, presence, ip, device_count
   - `game_devices`: id, user_id, device_id, system_language, timezone, gpu_img, gm_list

3. Save and refresh your browser!

## 🛡️ Security Notes

- Uses Supabase Row Level Security (RLS)
- Anon key provided has read-only access
- No sensitive data exposed in frontend code
- All queries go through Supabase security policies

## 📚 Learning Resources

- [Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding) - O(n log n) compression
- [Union-Find](https://en.wikipedia.org/wiki/Disjoint-set_data_structure) - Identity management
- [Inverted Index](https://en.wikipedia.org/wiki/Inverted_index) - Full-text search
- [Bloom Filter](https://en.wikipedia.org/wiki/Bloom_filter) - Duplicate detection
- [MinHash](https://en.wikipedia.org/wiki/MinHash) - Similarity estimation

## 🎓 Use Cases

- **Social Media Analytics** - Analyze profiles across platforms
- **Full-Text Search** - Search bios and captions efficiently
- **Fraud Detection** - Identify bot and duplicate accounts
- **Performance Optimization** - Compress and optimize data storage

## 📝 License

Project Chameleon - DSA Dashboard (2024)

## 🤝 Contributing

Contributions welcome! This is a learning project demonstrating:
- Real-time data processing
- DSA algorithm implementations
- Database integration
- Modern web development

## 📞 Support

For issues or questions:
1. Check the browser console for errors (F12)
2. Verify Supabase credentials are correct
3. Ensure database tables exist in Supabase
4. Check that RLS policies allow anonymous read access

---

Built with ❤️ for learning DSA principles through real-world applications.
