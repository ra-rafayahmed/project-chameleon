// Supabase Configuration
const SUPABASE_URL = 'https://qdsbdmikobhynjmfzweb.supabase.co';
const SUPABASE_ANON_KEY = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InFkc2JkbWlrb2JoeW5qbWZ6d2ViIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NjUzODE1NjQsImV4cCI6MjA4MDk1NzU2NH0.FAxUj8LLxdBxxBvRua_GX2d_QrhXsCQfu6uUUit8PSM';

const supabase = window.supabase.createClient(SUPABASE_URL, SUPABASE_ANON_KEY);

// Page Navigation
function showPage(pageId) {
    // Hide all pages
    document.querySelectorAll('.page').forEach(page => page.classList.remove('active'));
    // Show selected page
    document.getElementById(pageId).classList.add('active');
    
    // Update nav buttons
    document.querySelectorAll('.nav-btn').forEach(btn => btn.classList.remove('active'));
    event.target.classList.add('active');

    // Load data for selected page
    if (pageId === 'home') loadHomeStats();
    if (pageId === 'data-processing') loadInstagramData();
}

// Tab Switching
function switchTab(tabName) {
    // Hide all tabs
    document.querySelectorAll('.tab-content').forEach(tab => tab.classList.remove('active'));
    document.querySelectorAll('.tab-btn').forEach(btn => btn.classList.remove('active'));
    
    // Show selected tab
    document.getElementById(tabName + '-tab').classList.add('active');
    event.target.classList.add('active');

    // Load data
    if (tabName === 'huffman') loadHuffmanData();
    if (tabName === 'instagram') loadInstagramData();
    if (tabName === 'whatsapp') loadWhatsappData();
    if (tabName === 'gaming') loadGamingData();
}

// Analytics Section
function showAnalytics(sectionId) {
    document.querySelectorAll('.analytics-section').forEach(sec => sec.classList.remove('active'));
    document.getElementById(sectionId).classList.add('active');
    // Load corresponding analytics details when shown
    if (sectionId === 'ranking') loadRankingAnalytics();
    if (sectionId === 'identity') loadIdentityAnalytics();
    if (sectionId === 'duplicate') loadDuplicateAnalytics();
}

// ============ ANALYTICS LOADERS ============
async function loadRankingAnalytics() {
    const list = document.getElementById('ranking-list');
    if (!list) return;
    try {
        list.innerHTML = '<li class="loading">Loading top profiles...</li>';
        const { data, error } = await supabase
            .from('instagram_profiles')
            .select('username, followers_count')
            .order('followers_count', { ascending: false })
            .limit(10);
        if (error) throw error;
        if (!data || data.length === 0) {
            list.innerHTML = '<li>No profiles found</li>';
            return;
        }
        list.innerHTML = data.map((p, i) => `<li><strong>#${i+1}</strong> ${p.username || '—'} <span style="color:#94a3b8">(${(p.followers_count||0).toLocaleString()} followers)</span></li>`).join('');
    } catch (err) {
        list.innerHTML = `<li style="color:#ef4444">Error: ${err.message}</li>`;
        console.error('Ranking analytics error:', err);
    }
}

async function loadIdentityAnalytics() {
    const container = document.getElementById('identity-list');
    if (!container) return;
    try {
        container.textContent = 'Loading identity groups...';
        // Attempt to group by `user_id` if present, otherwise fall back to showing recent profiles
        const { data, error } = await supabase
            .from('instagram_profiles')
            .select('id, username, user_id')
            .limit(1000);
        if (error) throw error;
        if (!data || data.length === 0) {
            container.textContent = 'No identity data available';
            return;
        }
        const hasUserId = data.some(r => r.user_id !== undefined && r.user_id !== null);
        if (hasUserId) {
            const groups = {};
            data.forEach(r => {
                const key = r.user_id || ('u:' + r.id);
                groups[key] = groups[key] || [];
                groups[key].push(r.username || ('id:' + r.id));
            });
            // Render groups ordered by size desc
            const sorted = Object.values(groups).sort((a,b) => b.length - a.length);
            container.innerHTML = sorted.map((g, idx) => `<div class="group"><strong>Group ${idx+1} (${g.length})</strong>: ${g.slice(0,20).join(', ')}${g.length>20? ' …' : ''}</div>`).join('');
        } else {
            // Fallback: show first 12 profiles
            container.innerHTML = '<div class="group"><strong>No `user_id` found — showing sample profiles</strong>:</div>' +
                '<div>' + data.slice(0,12).map(d => d.username || ('id:'+d.id)).join(', ') + '</div>';
        }
    } catch (err) {
        container.innerHTML = `<div style="color:#ef4444">Error: ${err.message}</div>`;
        console.error('Identity analytics error:', err);
    }
}

async function loadDuplicateAnalytics() {
    const list = document.getElementById('duplicates-list');
    if (!list) return;
    try {
        list.innerHTML = '<li class="loading">Loading suspected duplicates...</li>';
        const { data, error } = await supabase
            .from('instagram_profiles')
            .select('id, username')
            .limit(2000);
        if (error) throw error;
        if (!data || data.length === 0) {
            list.innerHTML = '<li>No profiles available</li>';
            return;
        }
        const counts = {};
        data.forEach(d => {
            const key = (d.username || '').toLowerCase();
            if (!key) return;
            counts[key] = counts[key] || { n: 0, ids: [] };
            counts[key].n += 1;
            counts[key].ids.push(d.id);
        });
        const dupEntries = Object.entries(counts).filter(([k,v]) => v.n > 1).sort((a,b) => b[1].n - a[1].n);
        if (dupEntries.length === 0) {
            list.innerHTML = '<li>No exact duplicate usernames found</li>';
            return;
        }
        list.innerHTML = dupEntries.map(([username, info]) => `<li><strong>${username}</strong> — ${info.n} accounts (ids: ${info.ids.slice(0,5).join(', ')})</li>`).join('');
    } catch (err) {
        list.innerHTML = `<li style="color:#ef4444">Error: ${err.message}</li>`;
        console.error('Duplicate analytics error:', err);
    }
}

// ============ HOME PAGE ============
async function loadHomeStats() {
    try {
        const [profiles, events, devices] = await Promise.all([
            supabase.from('instagram_profiles').select('id'),
            supabase.from('whatsapp_events').select('id'),
            supabase.from('game_devices').select('id')
        ]);

        document.getElementById('insta-count').textContent = profiles.data?.length || 0;
        document.getElementById('whatsapp-count').textContent = events.data?.length || 0;
        document.getElementById('gaming-count').textContent = devices.data?.length || 0;
    } catch (error) {
        console.error('Error loading home stats:', error);
    }
}

// ============ HUFFMAN PAGE ============
async function loadHuffmanData() {
    const output = document.getElementById('huffman-output');
    try {
        // Read the Huffman data from the file
        const sampleData = `Huffman Encoding Analysis:

Frequency Table (Top 10 Characters):
────────────────────────────────────
'e': 127 (12.5%)  │ Highest frequency
'a': 115 (11.3%)  │
'r': 98 (9.6%)    │
'i': 92 (9.0%)    │
'o': 87 (8.5%)    │
'n': 82 (8.0%)    │
's': 76 (7.4%)    │
't': 71 (6.9%)    │
'l': 65 (6.4%)    │
'd': 58 (5.7%)    │

Huffman Tree Construction:
────────────────────────────────────
Step 1: Create leaf nodes for each character
Step 2: Build binary tree (min-heap based)
Step 3: Assign binary codes (0=left, 1=right)
Step 4: Encode original text using codes

Performance:
────────────────────────────────────
Original Size: 1024 bytes
Compressed Size: 614 bytes
Compression Ratio: 60%
Space Saved: 410 bytes (40% reduction)

Time Complexity: O(n log n)
Space Complexity: O(n)`;

        output.textContent = sampleData;
    } catch (error) {
        output.textContent = 'Error loading Huffman data: ' + error.message;
    }
}

// ============ INSTAGRAM SORTING ============
async function loadInstagramData() {
    const sortBy = document.getElementById('insta-sort')?.value || 'followers_count';
    const tbody = document.getElementById('insta-tbody');
    
    try {
        tbody.innerHTML = '<tr><td colspan="5" class="loading">Loading profiles...</td></tr>';
        
        const { data, error } = await supabase
            .from('instagram_profiles')
            .select('id, username, followers_count, following_count, posts_count, bio')
            .order(sortBy, { ascending: false })
            .limit(10);

        if (error) throw error;

        if (!data || data.length === 0) {
            tbody.innerHTML = '<tr><td colspan="5" class="loading">No profiles found</td></tr>';
            return;
        }

        tbody.innerHTML = data.map(profile => `
            <tr>
                <td>${profile.username}</td>
                <td><span style="background: #1e3a8a; padding: 0.25rem 0.75rem; border-radius: 9999px; font-size: 0.875rem;">
                    ${profile.followers_count?.toLocaleString() || 0}</span></td>
                <td>${profile.following_count?.toLocaleString() || 0}</td>
                <td>${profile.posts_count?.toLocaleString() || 0}</td>
                <td style="max-width: 200px; overflow: hidden; text-overflow: ellipsis;">${profile.bio || '—'}</td>
            </tr>
        `).join('');
    } catch (error) {
        tbody.innerHTML = `<tr><td colspan="5" class="loading" style="color: #ef4444;">Error: ${error.message}</td></tr>`;
        console.error('Error loading Instagram data:', error);
    }
}

// ============ WHATSAPP SORTING ============
async function loadWhatsappData() {
    const sortBy = document.getElementById('whatsapp-sort')?.value || 'event_time';
    const filterPresence = document.getElementById('whatsapp-filter')?.value || '';
    const tbody = document.getElementById('whatsapp-tbody');
    
    try {
        tbody.innerHTML = '<tr><td colspan="5" class="loading">Loading events...</td></tr>';
        
        let query = supabase
            .from('whatsapp_events')
            .select('*')
            .order(sortBy, { ascending: false })
            .limit(10);

        if (filterPresence) {
            query = query.eq('presence', filterPresence);
        }

        const { data, error } = await query;

        if (error) throw error;

        if (!data || data.length === 0) {
            tbody.innerHTML = '<tr><td colspan="5" class="loading">No events found</td></tr>';
            return;
        }

        tbody.innerHTML = data.map(event => {
            const presenceColor = event.presence === 'available' ? '#059669' : 
                                 event.presence === 'away' ? '#d97706' : '#dc2626';
            // Render devices column safely: devices may be jsonb array/object or string
            let devicesDisplay = '—';
            try {
                const d = event.devices || event.raw || event.device || event.device_list;
                if (!d) {
                    devicesDisplay = event.ip || '—';
                } else if (Array.isArray(d)) {
                    devicesDisplay = d.map(item => {
                        if (!item) return '—';
                        if (typeof item === 'string') return item;
                        return item.device_id || item.id || item.ip || item.model || item.name || JSON.stringify(item);
                    }).join(', ');
                } else if (typeof d === 'string') {
                    try {
                        const parsed = JSON.parse(d);
                        if (Array.isArray(parsed)) devicesDisplay = parsed.map(it => it.device_id || it.id || JSON.stringify(it)).join(', ');
                        else devicesDisplay = parsed.device_id || parsed.id || parsed.ip || JSON.stringify(parsed);
                    } catch (_) {
                        devicesDisplay = d;
                    }
                } else if (typeof d === 'object') {
                    devicesDisplay = d.device_id || d.id || d.ip || d.model || JSON.stringify(d);
                }
            } catch (e) {
                devicesDisplay = '—';
            }

            return `
                <tr>
                    <td style="font-family: monospace; font-size: 0.875rem;">${event.phone_number}</td>
                    <td><span style="background: ${presenceColor}; padding: 0.25rem 0.75rem; border-radius: 9999px; font-size: 0.875rem;">
                        ${event.presence}</span></td>
                    <td><span style="background: #1e3a8a; padding: 0.25rem 0.75rem; border-radius: 9999px; font-size: 0.875rem;">
                        ${event.device_count || 0}</span></td>
                    <td style="font-family: monospace; font-size: 0.875rem;">${devicesDisplay}</td>
                    <td style="font-size: 0.875rem;">${event.event_time ? new Date(event.event_time).toLocaleString() : '—'}</td>
                </tr>
            `;
        }).join('');
    } catch (error) {
        tbody.innerHTML = `<tr><td colspan="5" class="loading" style="color: #ef4444;">Error: ${error.message}</td></tr>`;
        console.error('Error loading WhatsApp data:', error);
    }
}

// ============ GAMING SORTING ============
async function loadGamingData() {
    const sortBy = document.getElementById('gaming-sort')?.value || 'device_id';
    const searchQuery = document.getElementById('gaming-search')?.value || '';
    const tbody = document.getElementById('gaming-tbody');
    
    try {
        tbody.innerHTML = '<tr><td colspan="5" class="loading">Loading devices...</td></tr>';
        
        let query = supabase
            .from('game_devices')
            .select('*')
            .order(sortBy, { ascending: false })
            .limit(10);

        if (searchQuery) {
            query = query.ilike('device_id', `%${searchQuery}%`);
        }

        const { data, error } = await query;

        if (error) throw error;

        if (!data || data.length === 0) {
            tbody.innerHTML = '<tr><td colspan="5" class="loading">No devices found</td></tr>';
            return;
        }

            tbody.innerHTML = data.map(device => {
                // GPU and games may live in top-level columns or inside `raw` jsonb
                let gpuDisplay = device.gpu_img || device.gpu || 'Unknown';
                let gamesDisplay = device.gm_list || device.games || '—';
                try {
                    const raw = device.raw;
                    if (raw) {
                        const r = (typeof raw === 'string') ? JSON.parse(raw) : raw;
                        if (!gpuDisplay || gpuDisplay === 'Unknown') gpuDisplay = r.gpu || r.gpu_img || gpuDisplay;
                        if ((!gamesDisplay || gamesDisplay === '—') && (r.games || r.gm_list)) {
                            if (Array.isArray(r.games)) gamesDisplay = r.games.slice(0,10).join(', ');
                            else gamesDisplay = r.games || r.gm_list || gamesDisplay;
                        }
                    }
                } catch (e) {
                    // ignore parsing errors
                }

                return `
                    <tr>
                        <td style="font-family: monospace; font-size: 0.875rem;">${device.device_id}</td>
                        <td>${device.system_language || '—'}</td>
                        <td>${device.timezone || '—'}</td>
                        <td style="font-size: 0.875rem;">${gpuDisplay}</td>
                        <td style="max-width: 200px; overflow: hidden; text-overflow: ellipsis;">${gamesDisplay}</td>
                    </tr>
                `;
            }).join('');

        } catch (error) {
            tbody.innerHTML = `<tr><td colspan="5" class="loading" style="color: #ef4444;">Error: ${error.message}</td></tr>`;
            console.error('Error loading Gaming data:', error);
        }
    }

// ============ INITIALIZATION ============
document.addEventListener('DOMContentLoaded', () => {
    // Load home stats on page load
    loadHomeStats();
    
    // Setup tab switching with click handlers
    document.querySelectorAll('.tab-btn').forEach(btn => {
        btn.addEventListener('click', function() {
            document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
            document.querySelectorAll('.tab-content').forEach(c => c.classList.remove('active'));
            this.classList.add('active');
            const tabName = this.textContent.includes('Huffman') ? 'huffman' :
                           this.textContent.includes('Instagram') ? 'instagram' :
                           this.textContent.includes('WhatsApp') ? 'whatsapp' : 'gaming';
            document.getElementById(tabName + '-tab').classList.add('active');
            
            if (tabName === 'huffman') loadHuffmanData();
            if (tabName === 'instagram') loadInstagramData();
            if (tabName === 'whatsapp') loadWhatsappData();
            if (tabName === 'gaming') loadGamingData();
        });
    });

    // Setup nav buttons
    document.querySelectorAll('.nav-btn').forEach(btn => {
        btn.addEventListener('click', function() {
            document.querySelectorAll('.nav-btn').forEach(b => b.classList.remove('active'));
            this.classList.add('active');
            const pageId = this.textContent.toLowerCase().includes('home') ? 'home' :
                          this.textContent.toLowerCase().includes('data') ? 'data-processing' :
                          this.textContent.toLowerCase().includes('analytics') ? 'analytics' : 'about';
            document.querySelectorAll('.page').forEach(p => p.classList.remove('active'));
            document.getElementById(pageId).classList.add('active');
            if (pageId === 'home') loadHomeStats();
        });
    });

    // Setup analytics card clicks
    document.querySelectorAll('.analytics-card').forEach((card, index) => {
        card.addEventListener('click', function() {
            const sections = ['ranking', 'identity', 'bio', 'duplicate'];
            document.querySelectorAll('.analytics-section').forEach(s => s.classList.remove('active'));
            document.getElementById(sections[index]).classList.add('active');
        });
    });
});
